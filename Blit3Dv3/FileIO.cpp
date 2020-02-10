#include "TileMap.h"
#include "FileIO.h"
#include "Camera.h"
#include <iostream>
#include <fstream>

extern TileMap* map;
extern MessageState messageState;
extern Camera2D* camera;
extern int TILESIZE;

//David
//enum DTileType { EASY_PATH, WALL, SPAWN, DESTINATION };

//Alex
enum ATileType { TRAVERSABLE, OBSTACLE, DESTINATION, SPAWN = 4};

short tileConversionTable(short tile)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	tileConversionTable
	//
	//Method parameters	:	short tile
	//
	//Method return		:	short
	//
	//Synopsis			:   Converts the tiles values of other students to the tile values used by this application
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/20/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	switch (tile)
	{
	case SPAWN:
		return (short)TileType::SPAWN;
	case DESTINATION:
		return (short)TileType::DESTINATION;
	case OBSTACLE:
		return (short)TileType::WALL;
	default:
		return (short)TileType::EASY_PATH;
	}
}

bool FileIO::LoadMap(std::string filename)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	LoadMap
	//
	//Method parameters	:	std::string filename
	//
	//Method return		:	void
	//
	//Synopsis			:   Loads a tile map from a mapfile
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

	std::ifstream mapfile;																						//Map File Reader Variables
	unsigned short posX, posY;
	short tileX, tileY;
	short width, height;
	short tileData;
	ShortNode* node;

	map->clearMap(true);																						//Clears the map before loading the new map
	
	mapfile.open(filename);																						//Opens Map File
	
	if (!mapfile.is_open())																						//Checks to see if the File was successfully opened
	{
		messageState = MessageState::CANTLOAD;

		std::cout << " Can't open " + filename + "!" << std::endl;												//Displays error message and terminates the application
		return false;
	}

	mapfile >> width;																							//Retrieves the ListSize of the map that's being loaded
	mapfile >> height;
	
	if (map->getWidth() != width || map->getHeight() != height)													//Checks if the map ListSize is the same ListSize as the loaded map
	{
		delete map;																								//If not, it deletes the old map, and creates a new map with
		map = new TileMap(width, height);																		//the dimensions of the map loaded from the mapfile

		camera->minY = blit3D->screenHeight * 1.5f - map->getHeight() * TILESIZE;
		camera->maxX = map->getWidth() * TILESIZE - blit3D->screenWidth * 0.5f;
	}

	posX = width;
	posY = height;

	mapfile >> tileData;																						//Retrieves the initial tile tileType from the file
	mapfile >> posX;																							//Retrieves the initial position tileType from the file
	mapfile >> posY;
	
	//tileType = tileConversionTable(tileType);																	//Converts the tiles

	for (tileX = 0; tileX < width; tileX++)																		//Cycles through each of the tiles in the file
	{
		node = map->getNode(tileX, 0);									

		for (tileY = 0; tileY < height; tileY++)
		{
			if (tileX == posX && tileY == posY)
			{
				//Use this if any of the tile indexes get changed
				//if (tileData == 3) tileData = 4;

				map->setTileType(node, tileData);

				mapfile >> tileData;																			//Retrieves the tileValue from the text file
				mapfile >> posX;																				//Retrieves the position tileType from the text file
				mapfile >> posY;	

				//tileType = tileConversionTable(tileType);														//Converts the type values from other students
			}
			node = node->next;																					//Shifts to the next node in the column
		}
	}
	
	mapfile.close();																							//Closes the Map File

	messageState = MessageState::MAPLOADED;

	return true;																								//Returns a bool indicating weather or not the map
																												//file was successfully loaded
}


bool FileIO::SaveMap(std::string filename)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	SaveMap
	//
	//Method parameters	:	std::string filename
	//
	//Method return		:	bool
	//
	//Synopsis			:   Saves the tile map into a file
	//
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/21/2018		J. Bordage				Initial Setup
	//						10/22/2018		J. Bordage				Optimized node retrieval
	//
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	std::ofstream mapfile;																						//Map File Variables
	short tileX, tileY;
	short width, height;
	short tileData;
	ShortNode* node;

	map->clearMap(false);																						//Deletes any travelled tiles

	mapfile.open(filename);																						//Opens the map file

	if (!mapfile.is_open())																						//Error check to see if the file successfully opened
	{
		messageState = MessageState::CANTSAVE;
		std::cout << " Can't open " + filename + "!" << std::endl;

		return false;
	}

	width = map->getWidth();
	height = map->getHeight();

	mapfile << std::to_string(width) + "\n";																	//Writes the map dimensions to the map text file
	mapfile << std::to_string(height) + "\n";

	for (tileX = 0; tileX < width; tileX++)																		//Cycles through each of the tiles in the file
	{
		node = map->getNode(tileX, 0);																			//Retrieves the tile nodes

		for (tileY = 0; tileY < height; tileY++)
		{
			tileData = map->getTileType(node);																	//Gets the top tileMap node of each column

			if (tileData != (short)TileType::EASY_PATH)														//Checks that the tile being saved isn't a 
																												//traversable tile in order to reduce map file sizes
			{
				mapfile << std::to_string(tileData) + " ";														//Saves the tiledata
				mapfile << std::to_string(tileX) + " ";
				mapfile << std::to_string(tileY) + "\n";
			}

			node = node->next;																					//Shifts to the next node in the column
		}
	}

	mapfile.close();																							//Closes the map file

	messageState = MessageState::MAPSAVED;

	return true;
}