#pragma once
#include "Vector2Int.h"
#include <iostream>
#include "Definitions.h"
#include "TileMap.h"

class Node
{
private:

public:
	Vector2Int tilePos;

	Node* Parent;
	
	int heuristicValue;

	int gCost;
	int hCost;

	int fCost() const
	{
		return gCost + hCost;
	}

	Node()
	{
		tilePos = Vector2Int(-1, -1);
		Parent = NULL;
		gCost = 0;
		hCost = 0;
		heuristicValue = 1;
	}

	Node(Vector2Int gridPos, ShortNode* node)
	{
		tilePos = gridPos;
		Parent = NULL;
		gCost = 0;
		hCost = 0;

		switch (node->tileType)
		{
		default:
		case (int)TileType::EASY_PATH:
			heuristicValue = 1;
			break;
		case (int)TileType::MEDIUM_PATH:
			heuristicValue = 3;
			break;
		case (int)TileType::HARD_PATH:
			heuristicValue = 5;
			break;
		case (int)TileType::VERYHARD_PATH:
			heuristicValue = 7;
			break;
		}
	}

	bool operator ==(const Node* otherNode) const
	{
		std::cout << "Comparing Nodes" << std::endl;

		//Checks that the position is identical
		return ((this->tilePos.x == otherNode->tilePos.x) && (this->tilePos.y == otherNode->tilePos.y));
	}
};