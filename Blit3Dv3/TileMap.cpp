#include "TileMap.h"
#include "Definitions.h"
#include "Random.h"
#include <iostream>
#include <Blit3D.h>
#include "Pathfinder.h"

extern enum GameMode;
extern GameMode gameMode;

ShortNode::ShortNode()
{
	tileState = (short)TileState::NONE;
	tileType = NULL;
	next = NULL;
}

ShortNode::ShortNode(short _data)
{
	tileState = (short)TileState::NONE;
	tileType = _data;
	next = NULL;
}

ShortNode::~ShortNode()
{
	if (next != NULL)
		delete next;
}


PointNode::PointNode()
{
	data = NULL;
	next = NULL;
}

PointNode::~PointNode()
{
	if (data != NULL)
		delete data;

	if (next != NULL)
		delete next;
}

Path::Path()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Path
	//
	//Method parameters	:	short positionX, short positionY
	//
	//Method return		:	void
	//
	//Synopsis			:   Retains the tileType for each node in the rover's final path
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	data = NULL;
	next = NULL;
	posX = NULL;
	posY = NULL;
}

Path::Path(short positionX, short positionY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Path
	//
	//Method parameters	:	short positionX, short positionY
	//
	//Method return		:	void
	//
	//Synopsis			:   Retains the tileType for each node in the rover's final path
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	data = NULL;
	next = NULL;
	posX = positionX;
	posY = positionY;
}

Path::~Path()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	~Path
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Deconstructor for the path object
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	data = NULL;
	if (next != NULL)
	{
		PointNode* pathNode = next;

		pathNode->data = NULL;																					//Wipes the shortNode pointer tileType so it
		while (pathNode->next != NULL)																			//doesn't start deleting chunks of the map
		{
			pathNode = pathNode->next;
			pathNode->data = NULL;
		}
		pathNode = NULL;
	}

	posX = NULL;																								//Wipes position values
	posY = NULL;
}

TileMap::TileMap(short sizeX, short sizeY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	head = new PointNode();																						//Creates a tilemap head node

	width = sizeX;																								//Saves the dimensions of the map
	height = sizeY;

	makeMap(sizeX, sizeY);																						//Generates the remaining map nodes
}

TileMap::~TileMap()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	delete head;																								//Starts deleting the map from the head
}

void TileMap::makeMap(short sizeX, short sizeY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/17/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized node creation
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	short x, y;
	PointNode* node = head;
	ShortNode* shortNode;

	for (x = 0; x < sizeX; x++)
	{
		node->data = new ShortNode((short)TileType::EASY_PATH);
		shortNode = node->data;

		for (y = 0; y < sizeY; y++)
		{
			shortNode->next = new ShortNode((short)TileType::EASY_PATH);
			shortNode = shortNode->next;
		}

		if (x < sizeX - 1)
		{
			node->next = new PointNode();
			node = node->next;
		}
	}
}

void TileMap::generateMap(short tRatio, short oRatio)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/20/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized node retrieval
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	ShortNode* node;
	short xIndex, yIndex;
	Random rng;

	for (xIndex = 0; xIndex < width; xIndex++)
	{
		node = getNode(xIndex, 0);

		for (yIndex = 0; yIndex < height; yIndex++)
		{
			node->tileType = rng.genTileValue(tRatio, oRatio);

			node = node->next;
		}
	}
}

void TileMap::clearMap(bool clearEverything)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/19/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized node retrieval
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	short xIndex, yIndex;
	ShortNode* node;
	
	for (xIndex = 0; xIndex < width; xIndex++)
	{
		node = getNode(xIndex, 0);

		for (yIndex = 0; yIndex < height; yIndex++)
		{
			if (clearEverything)
			{
				node->tileType = (short)TileType::EASY_PATH;
			}

			if (node->tileState != (short)TileState::NONE)
				node->tileState = (short)TileState::NONE;

			node = node->next;
		}
	}
}

bool TileMap::findTile(short &xIndex, short &yIndex, short tileType)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/19/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized node retrieval
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	ShortNode* node;
	int tile;

	for (xIndex = 0; xIndex < width; xIndex++)
	{
		node = getNode(xIndex, 0);

		for (yIndex = 0; yIndex < height; yIndex++)
		{
			tile = node->tileType;

			if (tile == tileType)
			{
				//xIndex = tileState->tilePos.x;
				//yIndex = tileState->tilePos.y;

				return true;
			}

			node = node->next;
		}
	}

	return false;
}

std::vector<Path> TileMap::findTiles(short tileType)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/19/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized node retrieval
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	std::vector<Path> nodeList;
	short xIndex, yIndex;
	ShortNode* node;

	for (xIndex = 0; xIndex < width; xIndex++)
	{
		node = getNode(xIndex, 0);

		for (yIndex = 0; yIndex < height; yIndex++)
		{
			if (node->tileType == tileType)
			{
				Path pathNode = Path(xIndex, yIndex);
				pathNode.data = node;

				nodeList.push_back(pathNode);
			}

			node = node->next;
		}
	}

	return nodeList;
}

ShortNode* TileMap::getNode(short posX, short posY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	short x, y;
	PointNode* pNode = head;
	ShortNode* node;

	for (x = 0; x < posX; x++)
	{
		if(pNode->next != NULL)
			pNode = pNode->next;
	}

	node = pNode->data;

	for (y = 0; y < posY; y++)
	{
		if (node->next != NULL)
			node = node->next;
	}

	return node;
}

short TileMap::getTile(short posX, short posY)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	return getNode(posX, posY)->tileType;
}

short TileMap::getTileType(ShortNode* node)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	return node->tileType;
}

void TileMap::setTileType(ShortNode* node, short value)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	void main()
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Starts the hangman game
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/22/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	node->tileType = value;
}

void TileMap::setTileType(Vector2Int position, short value)
{
	setTileType(getNode(position.x, position.y), value);
}

short TileMap::getWidth()
{
	return width;
}

short TileMap::getHeight()
{
	return height;
}

