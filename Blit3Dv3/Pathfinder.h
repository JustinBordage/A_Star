#pragma once
#include "TileMap.h"
#include "Definitions.h"
#include "TileList.h"
#include "Vector2Int.h"
#include <Blit3D.h>

extern TileMap* map;

class Pathfinder
{
private:
	short destX, destY;
	void savePath(Vector2Int position);

public:
	TileList* openList;
	TileList* closedList;
	void processNeighbors(Node* currentNode, const Vector2Int& startPos);
	std::vector<Node*> GetNeighborNodes(Node* currNode);
	void getFinalPath(Node* start, Node* end);

	Path* trail;
	Pathfinder();
	~Pathfinder();

	bool findPath(Vector2Int startPos, Vector2Int endPos);
};