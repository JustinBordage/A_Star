#pragma once
#include <vector>
#include <string>
#include "TileMap.h"

class FileIO
{
public:
	bool LoadMap(std::string filename);
	bool SaveMap(std::string filename);
};