/*
Example program that shows how to display tileType maps
*/



#include "Blit3D.h"
#include "TileMap.h"
#include "Camera.h"
#include "Pathfinder.h"
#include "FileIO.h"
#include "Definitions.h"
#include "LinkedList.h"
#include "Vector2Int.h"
#include "MenuManager.h"

Blit3D *blit3D = NULL;

//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

//GLOBAL DATA
//sprite pointers
Sprite *tilemap = NULL;
Sprite *background = NULL;
Sprite *background_Values = NULL;
Sprite *Menu_Backsplash = NULL;
AngelcodeFont *afont = NULL;

std::vector<Sprite *> tileList;
std::vector<Sprite *> optionList;

// Pathfinder Variables
int TILESIZE = 20;
int HALFTILE = TILESIZE / 2;
int terrainHueristic = 1;

enum GameMode { EDITING, PATHFINDING, PATHDRAWING };
GameMode gameMode;

TileMap* map = NULL;
Camera2D* camera;
Pathfinder* rover;
Path* pathNode;

glm::vec2 oldCamPos = glm::vec2(0.f, 0.f);
glm::vec2 camPos = glm::vec2(48 * TILESIZE, 27.f * TILESIZE);
glm::vec2 panDir = glm::vec2(0.0f, 0.0f);
glm::vec2 BG_Size;

bool debugMode = true;
bool showHeuristics = false;
bool searched = false;
bool panning = false;
short editTileNum = 0;
int tiles = 0;

// Toast messaging variables
MessageState messageState = MessageState::NONE;
MessageState prevMsgState = MessageState::NONE;
bool renewMsgTimer = false;
float messageTimer = 0.f;

// Timing delay for pathdrawing
short msDelay = 30;
float stepDelay = msDelay * 0.001f;
float stepClock = 0.f;

//MenuBuilder Global Data
bool LMB = false;
bool paused = false;
Vector2Int cursorPos;
Menu menu;

AngelcodeFont * defaultTextFont[3] = { NULL };
Sprite *defaultBtnSprites[3] = { NULL };
Sprite *defaultSliderSprites[2] = { NULL };
Sprite *defaultBacksplash = NULL;

void Init()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Init
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructs and initializes all the objects and variables in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	afont = blit3D->MakeAngelcodeFontFromBinary32("Media\\Oswald_72.bin");										//Angelcode font

	tilemap = blit3D->MakeSprite(0, 0, 120, 20, "Media\\Rover_TileSheet.png");									//Loads the tileType sheet sprite

	BG_Size = glm::vec2(1920.f, 1080.f);																		//Initializes the maximum screen dimensions

	map = new TileMap(100, 100);																				//Creates the tilemap
	
	if (map->getWidth() < 96) BG_Size.x = map->getWidth() * TILESIZE;											//Modifies the screen dimensions should the map
	if (map->getHeight() < 54) BG_Size.y = map->getHeight() * TILESIZE;											//be smaller than 1920 pixels by 1080 pixels

	background = blit3D->MakeSprite(0, 0, BG_Size.x, BG_Size.y, "Media\\HardLight_Background.png");				//Loads the background sprite

	background_Values = blit3D->MakeSprite(0, 0, BG_Size.x, BG_Size.y, "Media\\Rover_Background_Values.png");	//Loads the background sprite with values

	Menu_Backsplash = blit3D->MakeSprite(0, 0, 1920.f, 240.f, "Media\\Backsplash.png");							//Loads the background sprite with values

	BG_Size *= 0.5f;																							//BG_Size is used during runtime to define the position of
																												//the background on the screen therefore the position is
																												//calculated in advance so as to minimize runtime cost
	
	for (int y = 0; y < 20 / TILESIZE; ++y) //1																	//Loads the individual 20x20 tiles
	{
		for (int x = 0; x < 200 / TILESIZE; ++x) //10
		{
			tileList.push_back(blit3D->MakeSprite(x * 20, y * 20, 20, 20, "Media\\Rover_TileSheet.png"));
		}

		for (int x = 0; x < 140 / TILESIZE; ++x) //7
		{
			optionList.push_back(blit3D->MakeSprite(x * 20, y * 20, 20, 20, "Media\\Rover_TileSelection.png"));
		}
	}

	menu.Init();																								//Initalizes all menu elements
	
	camera = new Camera2D();																					//Creates the camera object that follows the rover

	camera->minX = blit3D->screenWidth * 0.5f;																	//Sets the highest and lowest positions the camera can move to
	camera->minY = blit3D->screenHeight * 1.5f - map->getHeight() * TILESIZE - 240.f;
	camera->maxX = map->getWidth() * TILESIZE - blit3D->screenWidth * 0.5f;
	camera->maxY = blit3D->screenHeight * 0.5f;

	gameMode = EDITING;																							//Sets the gamemode to editing

}

void DeInit(void)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DeInit
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Deconstructs all the objects in the pathfinder application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	delete camera;																								//Deletes the camera object

	if (rover != NULL) delete rover;																			//If the application was closed while the
																												//rover was pathfinding or displaying the
																												//final path it gets deleted here

	if (map != NULL) delete map;																				//Deletes the tilemap that was created

	menu.Deinit();
}

void Update(double seconds)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Update
	//
	//Method parameters	:	double seconds
	//
	//Method return		:	void
	//
	//Synopsis			:   Updates all the visual element tileType in the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (!paused)
	{
		if (messageState != MessageState::NONE)
			messageTimer += seconds;

		switch (gameMode)
		{
			case EDITING:
			{
				if (!panning) panDir = glm::vec2(0.f, 0.f);																		//Stops the camera pan
				else
				{
					camPos += panDir;																							//Pans the camera

					if (camPos.x <= camera->minX) camPos.x = camera->minX;														//Bounds Check
					if (camPos.x >= camera->maxX) camPos.x = camera->maxX;
					if (camPos.y <= camera->minY) camPos.y = camera->minY;
					if (camPos.y >= camera->maxY) camPos.y = camera->maxY;
				}

				if (LMB)
				{
					if (cursorPos.y - (27.f * TILESIZE - camPos.y) <= 840.f)
					{
						if (searched)
						{
							map->clearMap(false);
							searched = false;
						}

						int x = (cursorPos.x - cursorPos.x % TILESIZE) / TILESIZE;													//Calculates the tileType number
						int y = (cursorPos.y - cursorPos.y % TILESIZE) / TILESIZE;

						if (y >= 0 && y < map->getHeight() && x >= 0 && x < map->getWidth())										//Checks if the cursor is above a valid tileType
						{
							map->setTileType(map->getNode(x, y), editTileNum);														//Replaces the selected tileType
						}

						if (editTileNum == (short)TileType::SPAWN || editTileNum == (short)TileType::DESTINATION)					//Stops the repeating placement if the tiletype is either
						{																											//spawn point or the destination
							LMB = false;

							std::vector<Path> nodeList = map->findTiles(editTileNum);

							for (Path path : nodeList)
							{
								if (path.posX != x && path.posY != y)
								{
									path.data->tileType = (short)TileType::EASY_PATH;
								}
							}
						}
					}
				}
				break;
			}
			case PATHFINDING:
			{
				//Resets the tileType counter
				tiles = 0;
				editTileNum = 0;

				rover = new Pathfinder();

				pathNode = rover->trail;

				gameMode = PATHDRAWING;

				camera->Pan(0.f, 0.f);

				break;
			}
			case PATHDRAWING:
			{
				if (pathNode != NULL)
				{
					stepClock += seconds;

					if (stepClock >= stepDelay)
					{
						tiles++;
						camPos.x = pathNode->posX * TILESIZE;
						camPos.y = blit3D->screenHeight - pathNode->posY * TILESIZE;

						pathNode->data->tileState = (short)TileState::TRAVELLED;

						if (pathNode->next != NULL)
							pathNode = (Path*)pathNode->next;
						else
						{
							pathNode->data->tileState = (short)TileState::ROVER;
							delete rover;
							rover = NULL;
							pathNode = NULL;

							gameMode = EDITING;
						}

						stepClock -= stepClock < 1.f ? stepDelay : stepClock;
					}

					searched = true;
				}
				else
				{
					if (rover != NULL)
					{
						delete rover;
						rover = NULL;
						pathNode = NULL;
					}

					gameMode = EDITING;
				}
				break;
			}
		}
		
		camera->PanTo(camPos.x, camPos.y);
	}

	camera->Update(seconds);

	if (camera->posX != oldCamPos.x || camera->posY != oldCamPos.y)
	{
		glm::vec2 cameraPos = glm::vec2(camera->posX, camera->posY);

		menu.setMenuOffset(cameraPos);

		oldCamPos = cameraPos;
	}

	menu.Update(Vector2Int(cursorPos.x, 1080 - cursorPos.y), seconds);
}

void displayEditorHUD()
{
	if (gameMode != PATHFINDING)
	{
		((BlitLabel*)menu.UIList[0]->elementList[1])->SetText("Tiles: " + std::to_string(tiles));

		if (gameMode == EDITING)
		{
			//draw mouse coords
			short blockPosX = (short)(cursorPos.x - cursorPos.x % TILESIZE);
			short blockPosY = (short)(cursorPos.y - cursorPos.y % TILESIZE);

			BlitLabel* lblPosX = (BlitLabel*)menu.UIList[0]->elementList[2];
			BlitLabel* lblPosY = (BlitLabel*)menu.UIList[0]->elementList[3];

			if (cursorPos.y - (27.f * TILESIZE - camPos.y) <= 840.f)
			{
				lblPosX->SetText("X:" + std::to_string(blockPosX / TILESIZE));
				lblPosY->SetText("Y:" + std::to_string(blockPosY / TILESIZE));
			}
			else
			{
				lblPosX->SetText("");
				lblPosY->SetText("");
			}


			if (blockPosX / TILESIZE < map->getWidth() && blockPosY / TILESIZE < map->getHeight())
				optionList[editTileNum]->Blit(blockPosX + TILESIZE * 0.5f, (1080 - blockPosY) - TILESIZE * 0.5f);
		}
	}
}

void drawMap()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	drawMap
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Draws all the tilemap sprites
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized tilemap node retrieval
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	static glm::vec2 screenPos;

	glClearColor(0.5f, 0.0f, 0.5f, 0.0f);	//Clear colour: r,g,b,a
	// wipe the drawing surface Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->Draw();

	screenPos = glm::vec2(blit3D->screenWidth - BG_Size.x - camera->posX, blit3D->screenHeight - BG_Size.y - camera->posY);

	//Draw the background
	if(!showHeuristics)
		background->Blit(screenPos.x, screenPos.y);
	else
		background_Values->Blit(screenPos.x, screenPos.y);

	//displayEditorHUD();

	screenPos = glm::vec2((screenPos.x - blit3D->screenWidth * 0.5f )/ TILESIZE, -((screenPos.y - blit3D->screenHeight * 0.5f) / TILESIZE));

	short xIndex, yIndex, tileData;
	ShortNode* node;
	
	for (xIndex = screenPos.x; xIndex < screenPos.x + blit3D->screenWidth / TILESIZE; ++xIndex)
	{
		node = map->getNode(xIndex, (short) screenPos.y);

		for (yIndex = screenPos.y; yIndex < screenPos.y + blit3D->screenHeight / TILESIZE; ++yIndex)
		{
			tileData = node->tileType;

			if (tileData > 0)
			{
				tileList[tileData]->Blit(xIndex * TILESIZE + HALFTILE, blit3D->screenHeight - yIndex * TILESIZE - HALFTILE);

				if (showHeuristics)
				{
					if (tileData < 7)
					{
						optionList[tileData]->Blit(xIndex * TILESIZE + HALFTILE, blit3D->screenHeight - yIndex * TILESIZE - HALFTILE);
					}
				}
			}

			tileData = node->tileState;

			if (tileData != (short)TileState::NONE)
			{
				tileList[tileData]->Blit(xIndex * TILESIZE + HALFTILE, blit3D->screenHeight - yIndex * TILESIZE - HALFTILE);
			}

			if(node->next != NULL)
				node = node->next;
		}
	}
}

void displayMessages()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	displayMessages
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Displays the various info messages caused by various events
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/22/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	static std::string textMsg = "";
	static float msgFade;

	if (msgFade == 1.f && messageState != prevMsgState)
		messageTimer = 0.5f;

	if (messageTimer < 0.5f)
		msgFade = messageTimer * 2;
	else
	{
		if (messageTimer >= 0.5f && messageTimer <= 2.5f)
		{
			msgFade = 1.f;
		}
		else if (messageTimer > 2.5f)
		{
			msgFade = (3.f - messageTimer) * 2;

			if (messageTimer >= 3.f)
			{
				msgFade = 0.f;
				messageTimer = 0.f;
				messageState = MessageState::NONE;
				prevMsgState = MessageState::NONE;
			}
		}
	}

	switch (messageState)
	{
	case MessageState::NOPATH:
		textMsg = "Path Not Possible";
		break;
	case MessageState::PATHFOUND:
		textMsg = "Path Found!";
		break;
	case MessageState::NOSTART:
		textMsg = "No Start Point";
		break;
	case MessageState::NOEND:
		textMsg = "No Destination";
		break;
	case MessageState::CANTLOAD:
		textMsg = "Error - Cannot load file";
		break;
	case MessageState::CANTSAVE:
		textMsg = "Error - Save Unsuccessful";
		break;
	case MessageState::MAPLOADED:
		textMsg = "Map Loaded";
		break;
	case MessageState::MAPSAVED:
		textMsg = "Map Saved";
		break;
	case MessageState::STEPDELAY:
		textMsg = "Step Delay: " + std::to_string(msDelay);
		break;
	case MessageState::DEBUG:
		if (debugMode) textMsg = "Debug Draw On";
		else textMsg = "Debug Draw Off";
		break;
	default:
		textMsg = "";
		break;
	}

	afont->alpha = msgFade;
	afont->BlitText(960 - camera->posX - afont->WidthText(textMsg) * 0.5f, 100 -camera->posY, textMsg);
	prevMsgState = messageState;
}

void Draw(void)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Draw
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:	Draws the visual elements of the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	drawMap();

	displayEditorHUD();

	menu.Draw();

	if(messageState != MessageState::NONE)
		displayMessages();
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoInput
	//
	//Method parameters	:	int key, int scancode, int action, int mods
	//
	//Method return		:	void
	//
	//Synopsis			:   Performs various actions based on the key input of the user
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		menu.pause();
		//blit3D->Quit(); //start the shutdown sequence

	if (gameMode == EDITING)
	{
		if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
			gameMode = PATHFINDING;

		if (key == GLFW_KEY_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				panDir += glm::vec2(-TILESIZE * 10, 0.0f);
				panning = true;
			}
			else if (action == GLFW_RELEASE)
			{
				panning = false;
			}
		}

		if (key == GLFW_KEY_RIGHT)
		{
			if (action == GLFW_PRESS)
			{
				panDir += glm::vec2(TILESIZE * 10, 0.0f);
				panning = true;
			}
			else if (action == GLFW_RELEASE)
			{
				panning = false;
			}
		}

		if (key == GLFW_KEY_UP)
		{
			if (action == GLFW_PRESS)
			{
				panDir += glm::vec2(0.0f, TILESIZE * 10);
				panning = true;
			}
			else if (action == GLFW_RELEASE)
			{
				panning = false;
			}
		}

		if (key == GLFW_KEY_DOWN)
		{
			if (action == GLFW_PRESS)
			{
				panDir += glm::vec2(0.0f, -TILESIZE * 10);
				panning = true;
			}
			else if (action == GLFW_RELEASE)
			{
				panning = false;
			}
		}



		if (key == GLFW_KEY_V && action == GLFW_RELEASE)
		{
			showHeuristics = !showHeuristics;
		}

		if (key == GLFW_KEY_T && action == GLFW_RELEASE)
		{
			camPos.x = blit3D->screenWidth / 2;
			camPos.y = blit3D->screenHeight / 2;
		}

		if (key == GLFW_KEY_B && action == GLFW_RELEASE)
		{
			camPos.x = map->getWidth() * TILESIZE - blit3D->screenWidth * 0.5f;
			camPos.y = blit3D->screenHeight * 1.5f - map->getHeight() * TILESIZE;
		}

		if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
		{
			FileIO* mapIO = new FileIO();
			mapIO->LoadMap("Map1.txt");
			delete mapIO;
		}

		if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
		{
			FileIO* mapIO = new FileIO();
			mapIO->LoadMap("Map2.txt");
			delete mapIO;
		}

		if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
		{
			FileIO* mapIO = new FileIO();
			mapIO->LoadMap("Map3.txt");
			delete mapIO;
		}

		if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
		{
			FileIO* mapIO = new FileIO();
			mapIO->LoadMap("CustomMap.txt");
			delete mapIO;
		}

		//if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		//{
		//	FileIO* mapIO = new FileIO();
		//	mapIO->SaveMap("CustomMap.txt");
		//	delete mapIO;
		//}

		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			debugMode = !debugMode;

			messageState = MessageState::DEBUG;
		}

		if (key == GLFW_KEY_G && action == GLFW_RELEASE)
		{
			map->generateMap(7, 3);
		}

		if (key == GLFW_KEY_C && action == GLFW_RELEASE)
		{
			map->clearMap(true);
		}
	}

	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	{
		if (msDelay < 200)
		{
			msDelay += 10;
			stepDelay = msDelay * 0.001f;

			if (messageState != MessageState::NONE)
				messageTimer = 0.5f;

			messageState = MessageState::STEPDELAY;
		}
	}

	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	{
		if (msDelay > 0)
		{
			msDelay -= 10;
			stepDelay = msDelay * 0.001f;

			if (messageState != MessageState::NONE)
				messageTimer = 0.5f;

			messageState = MessageState::STEPDELAY;
		}
	}
}

void DoCursor(double x, double y)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoCursor
	//
	//Method parameters	:	double x, double y
	//
	//Method return		:	void
	//
	//Synopsis			:   Tracks the position of the cursor on the screen
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (gameMode == EDITING)
	{
		cursorPos.x = blit3D->screenWidth / blit3D->trueScreenWidth * (int)x - camera->posX;
		cursorPos.y = blit3D->screenHeight / blit3D->trueScreenHeight * (int)y + camera->posY;
	}
}

void DoMouseButton(int button, int action, int mods)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoMouseButton
	//
	//Method parameters	:	int button, int action, int mods
	//
	//Method return		:	void
	//
	//Synopsis			:   Does mouse button input
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (gameMode == EDITING)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			LMB = true;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			LMB = false;
		}
		/*
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			//Calculates the tileType number
			int x = (cursor_x - cursor_x % TILESIZE) / TILESIZE;
			int y = (cursor_y - cursor_y % TILESIZE) / TILESIZE;

			//Checks if the cursor is above a valid tileType
			if (y >= 0 && y < map->getHeight() && x >= 0 && x < map->getWidth())
				//Retrieves the selected tileType
				editTileNum = map->getTileData(map->getNode(x, y)) + 1;
		}*/
	}
}

void DoScrollWheel(double x, double y)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	DoScrollWheel
	//
	//Method parameters	:	double x, double y
	//
	//Method return		:	void
	//
	//Synopsis			:   Does scroll wheel input
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	editTileNum -= (short) y;
	if (editTileNum > 6) editTileNum = 6;
	if (editTileNum < 0) editTileNum = 0;
}

int main(int argc, char *argv[])
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	int argc, char *argv[]
	//
	//Method return		:	int (error code)
	//
	//Synopsis			:   Initiates the application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/01/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(11705);

	//Leak Codes
	/*
	*/

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 1920, 1080);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoCursor(DoCursor);
	blit3D->SetDoMouseButton(DoMouseButton);
	blit3D->SetDoScrollwheel(DoScrollWheel);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}












/*
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			debugMode = !debugMode;

			if (debugMode) std::cout << "Debug On" << std::endl;
			else std::cout << "Debug Off" << std::endl;
		}*/

		/*
		if (key == GLFW_KEY_M && action == GLFW_RELEASE)
		{
			map->clearMap(true);

			short x, y;

			for (y = 0; y <4; y++)
			{
				for (x = 0; x < mapWidth - 1; x++)
				{
					map->setTileData(map->getNode(x, 1 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x + 1, 3 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x, 5 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x + 1, 7 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x, 9 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x + 1, 11 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x, 13 + y * 17), (short)TileType::WALL);
					map->setTileData(map->getNode(x + 1, 15 + y * 17), (short)TileType::WALL);
				}
			}
		}*/