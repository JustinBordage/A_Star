#include "MenuFunctionality.h"
#include "MenuManager.h"
#include "TileMap.h"
#include "Blit3D.h"
#include "FileIO.h"

extern Blit3D *blit3D;
extern Menu menu;
extern AngelcodeFont *defaultTextFont[3];
extern int terrainHueristic;
extern TileMap *map;
extern bool searched;

extern enum GameMode { EDITING, PATHFINDING, PATHDRAWING };
extern GameMode gameMode;

//---------------------------------------------------------------------------------------------------------------------------------------------------
//															MENUS
//---------------------------------------------------------------------------------------------------------------------------------------------------

void scoreMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition)
{

}

void gameOverMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition)
{	
	//Displays the Final Score
	/*std::string scoreText = "Score: " + std::to_string(ship->score);
	textFont->BlitAutoText(menuPosition.x, menuPosition.y, scoreText);*/
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//															BUTTONS
//---------------------------------------------------------------------------------------------------------------------------------------------------

//---General Functionality---

void resume()
{
	//Unpauses the game
	paused = false;

	//Switches the UI and Menu Configurations
	menu.menuStatus = menu.NONE;
}

void back()
{
	//Returns to the previousMenu
	menu.menuStatus = menu.prevMenu;
}

void exit()
{
	//Exits the application
	blit3D->Quit();
}

//---Unique Functionality---

void start()
{
	//Resumes the game
	resume();
}

void settings()
{
	//Saves current menu (for back button)
	menu.prevMenu = menu.menuStatus;

	//Switches to the Settings Menu
	menu.menuStatus = menu.SETTINGSMENU;
}

void LoadMapOne()
{
	FileIO* mapIO = new FileIO();
	mapIO->LoadMap("Map1.txt");
	delete mapIO;
}

void LoadMapTwo()
{
	FileIO* mapIO = new FileIO();
	mapIO->LoadMap("Map2.txt");
	delete mapIO;
}

void LoadMapThree()
{
	FileIO* mapIO = new FileIO();
	mapIO->LoadMap("Map3.txt");
	delete mapIO;
}

void LoadCustomMap()
{
	FileIO* mapIO = new FileIO();
	mapIO->LoadMap("CustomMap.txt");
	delete mapIO;
}

void SaveCustomMap()
{
	FileIO* mapIO = new FileIO();
	mapIO->SaveMap("CustomMap.txt");
	delete mapIO;
}

void FindRoverPath()
{
	gameMode = PATHFINDING;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//															SLIDERS
//---------------------------------------------------------------------------------------------------------------------------------------------------

void HeuristicSlider(float value)
{
	terrainHueristic = (int) value;

	if (searched)
	{
		map->clearMap(false);
		searched = false;
	}
}

void sensitivitySlider(float percent)
{
	//Passes in a percentage modifier (1-100)
	//Set 1.f to the default value of the slider (50%)
	//sensitivity = 1.f * (sliderValue / 50);
}