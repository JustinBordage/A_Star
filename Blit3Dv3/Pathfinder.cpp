#include "Pathfinder.h"
#include "Camera.h"
#include "Node.h"
#include <math.h>

extern MessageState messageState;

extern Camera2D* camera;
extern glm::vec2 camPos;
float dtime, prevTime, elapsedTime;
extern Blit3D *blit3D;																								//Debug map drawing for pathfinder
extern void drawMap();
extern bool debugMode;
extern int terrainHueristic;
extern int TILESIZE;

#define SCREENWIDTH 1920.f
#define SCREENHEIGHT 1080.f

float roundToUnit(float floatToRound, float unit, float offset)
{
	//Adds an offset so it doesn't get chopped off in the rounding
	floatToRound += offset;

	//Calculates the number of units in the passed in float
	float numberOfUnits = floatToRound / unit;

	//Rounds to the nearest unit
	float roundedFloat = roundf(numberOfUnits) * unit;

	//Removes the offset
	roundedFloat -= offset;

	return roundedFloat;
}

void debugDrawer(short posX, short posY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	debugDrawer
	//
	//Method parameters	:	short PosX, short PosY
	//
	//Method return		:	void
	//
	//Synopsis			:   Renders all the tiles that were searched during the pathfinding sequence
	//
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/18/2018		J. Bordage				Initial Setup
	//
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//Draws the algorithm's searching
	if (debugMode)
	{
		//Calculates the new camera position
		glm::vec2 newPos;
		newPos.x = roundToUnit(posX * TILESIZE, SCREENWIDTH, SCREENWIDTH * 0.5f);
		newPos.y = roundToUnit(blit3D->screenHeight - posY * TILESIZE, SCREENHEIGHT, SCREENHEIGHT * 0.5f);
		static glm::vec2 oldPos = glm::vec2(0.f, 0.f);

		//Makes the debug drawing
		//less seizure inducing
		if (newPos != oldPos)
		{
			oldPos = newPos;
			Sleep(200);
		}

		//Calculates the elapsed time
		dtime = glfwGetTime();
		elapsedTime = dtime - prevTime;
		prevTime = dtime;

		//Sets the new camera Position
		camera->PanTo(newPos.x, newPos.y);

		//Updates the camera object
		camera->Update(elapsedTime);														

		//Draws the updated map
		drawMap();																								

		//Refreshes the screen
		glfwSwapBuffers(blit3D->window);																		
	}
}

short ToUnitVector(short value)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	ToUnitVector
	//
	//Method parameters	:	short value
	//
	//Method return		:	short (-1/0/1)
	//
	//Synopsis			:   Converts the passed in value to a single direction unit
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/15/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (value != 0)
		return value / abs(value);
	else
		return 0;
}

Pathfinder::Pathfinder()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Pathfinder
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructor for the pathfinder object
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/14/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	trail = NULL;
	short startX, startY;

	//Checks that start and end points are both present
	if (map->findTile(startX, startY, (short)TileType::SPAWN))
	{
		if (map->findTile(destX, destY, (short)TileType::DESTINATION))
		{
			//Clears the TileStates of all the tiles in the map (created by previous attempts)
			map->clearMap(false);

			//Finds the path to the destination
			if (findPath(Vector2Int(startX, startY), Vector2Int(destX, destY)))
			{
				//Displays a Toast Message telling the user that a path was found
				messageState = MessageState::PATHFOUND;
			}
			else
				//Displays a Toast Message telling the user that no path is possible
				messageState = MessageState::NOPATH;

			//Deletes the open and closed Lists
			delete openList;
			delete closedList;
		}
		else
			messageState = MessageState::NOEND;
	}
	else
		messageState = MessageState::NOSTART;
}

Pathfinder::~Pathfinder()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	~Pathfinder
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Deconstructor for the pathfinder object
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	if (trail != NULL)																							//Deletes the saved path
	{
		delete trail;
	}
}

void Pathfinder::savePath(Vector2Int position)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	savePath
	//
	//Method parameters	:	ShortNode* node, short posX, short posY
	//
	//Method return		:	void
	//
	//Synopsis			:   Saves the final path that was found by the pathfinder algorithm
	//						(Saves from the back to the front of the path)
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/17/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	Path* newNode = new Path(position.x, position.y);															//Creates a new path node and saves the tile coords

	newNode->data = map->getNode(position.x, position.y);														//Saves	the ShortNode for later use

	newNode->next = trail;																						//Saves the current node pointer in the new node

	trail = newNode;																							//Sets the current node to the new node
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//											A* METHOD
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Pathfinder::getFinalPath(Node* start, Node* end)
{
	//Saves the currentNode
	Node* currentNode = start;

	//Checks that the list hasn't gotten back to the end yet
	while (currentNode->tilePos != end->tilePos)
	{
		//Creates a list of the pathNodes
		savePath(currentNode->tilePos);																				//(Stores a pointer to the node in the tileMap, along with it's coordinates in the map)

		//Switches to the next node in the path
		currentNode = currentNode->Parent;
	}

	//Saves the last node in the path
	savePath(end->tilePos);
}

int GetManhattenDistance(Vector2Int posA, Vector2Int posB)
{
	//Calculates the Manhatten Distance (Absolute value of the delta)
	int deltaX = abs(posA.x - posB.x);
	int deltaY = abs(posA.y - posB.y);

	return deltaX + deltaY;
}

int GetEstimate(Vector2Int posA, Vector2Int posB, int hWeight)
{
	//Calculates the Manhatten Distance and applies the heuristic weight
	return GetManhattenDistance(posA, posB) * hWeight;
}

int GetMoveCost(Node* currentNode, Node* otherNode)
{
	//Crashes the application if the otherNode is not directly
	//adjacent to the current node (which it ALWAYS should be)
	assert(GetManhattenDistance(currentNode->tilePos, otherNode->tilePos) == 1);

	//Returns the gCost of what it's taken to get to the 'currentNode'
	//along with the 'actual' cost of moving to the 'otherNode'
	return currentNode->gCost + otherNode->heuristicValue;
}

std::vector<Node*> Pathfinder::GetNeighborNodes(Node* currNode)
{
	//Used for cutting the NeighborNode method down into a for loop
	//Instead of duplicating the code for each unique direction
	const short dirMod[5] = { 0, 1, 0, -1, 0 };

	//Variables
	std::vector<Node*> neighborList;
	Vector2Int tilePos;
	ShortNode* otherNode;

	//Cycles through each of the neighborNodes
	for (int neighbor = 0; neighbor < 4; neighbor++)
	{
		//Calculates the neighbor's position
		tilePos.x = currNode->tilePos.x + dirMod[neighbor + 1];
		tilePos.y = currNode->tilePos.y + dirMod[neighbor];

		//Checks that the coordinates are within the bounds of the map
		if (tilePos.x >= 0 && tilePos.x < map->getWidth())
		{
			if (tilePos.y >= 0 && tilePos.y < map->getHeight())
			{
				//Retrieves the neighbor node in the TileMap
				otherNode = map->getNode(tilePos.x, tilePos.y);

				//Ignores any nodes that are wall tiles
				if (otherNode->tileType != (short)TileType::WALL)
				{
					//Adds the neighbor node to the list
					neighborList.push_back(new Node(tilePos, otherNode));

					//Sets the node's state to an explored state
					otherNode->tileState = (short)TileState::EXPLORED;
				}
			}
		}
	}

	//Returns all the neighbor nodes that were not walls
	return neighborList;
}

void Pathfinder::processNeighbors(Node* currentNode, const Vector2Int& startPos)
{
	//Retrieves all the neighborNodes
	std::vector<Node*> neighborNodes = GetNeighborNodes(currentNode);

	for (Node* neighborNode : neighborNodes)
	{
		//Calculates the Move Cost
		int MoveCost = GetMoveCost(currentNode, neighborNode);

		//If the node is in one of the lists AND has a gCost lower than the MoveCost it 
		if ((closedList->Contains(neighborNode) || openList->Contains(neighborNode)) && neighborNode->gCost <= MoveCost)
		{
			//Deletes the node since this a different
			//instance of the node that is in the list
			delete neighborNode;
			continue;
		}

		//Sets the cost values to the node
		neighborNode->gCost = MoveCost;
		neighborNode->hCost = GetEstimate(neighborNode->tilePos, startPos, terrainHueristic);
		neighborNode->Parent = currentNode;

		//If it's contained in the closed list
		if (closedList->Contains(neighborNode))
		{
			//It removes the node from the
			//closed list for re-evaluation
			closedList->remove(neighborNode);
		}

		//If the node isn't already contained in the openList
		if (!openList->Contains(neighborNode))
		{
			//Inserts the node in it's sorted position in the list
			openList->sortedInsert(neighborNode);
		}
	}
}

bool Pathfinder::findPath(Vector2Int startPos, Vector2Int endPos)
{
	//Variables
	Node* currentNode;
	bool pathFound = false;
	Node* endNode = new Node(endPos, map->getNode(endPos.x, endPos.y));

	//Creates the open and closed lists
	openList = new TileList();
	closedList = new TileList();

	//Adds the starting node to the OpenList
	openList->add(endNode);

	//As long as there are still tiles to search
	while (openList->size() != 0)
	{
		//Pops the first (best) node in the open list
		currentNode = openList->get(0);
		openList->remove(0);

		//Adds the node to the closed list
		closedList->addUnique(currentNode);

		if(currentNode->tilePos == startPos)
		{
			//Creates a list of all the nodes in the found path
			getFinalPath(currentNode, endNode);
			pathFound = true;
			break;
		}

		//Retrieves and evaluates the neighborNodes
		processNeighbors(currentNode, startPos);

		//Draws the areas that the algorithm is currently searching as well as how it's searching
		debugDrawer(currentNode->tilePos.x, currentNode->tilePos.y);
	}

	//Returns weather or not the path was found
	return pathFound;
}