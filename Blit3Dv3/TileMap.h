#pragma once
#include "Definitions.h"
#include "Vector2Int.h"
#include "LinkedList.h"
#include <Blit3D.h>


// Linked list nodes
class ShortNode
{
public:
	ShortNode* next;
	short tileType;
	short tileState;

	ShortNode();
	ShortNode(short _data);
	~ShortNode();
};

class PointNode
{
public:
	PointNode* next;
	ShortNode* data;

	PointNode();
	~PointNode();
};

// Datastructure that contains the final path
class Path : public PointNode
{
public:
	short posX, posY;

	Path();
	Path(short positionX, short positionY);
	~Path();
};

// Manager class that controls the linked list nodes
class TileMap
{
private:
	PointNode* head;
	short width, height;
	
	void makeMap(short sizeX, short sizeY);

public:
	TileMap(short sizeX, short sizeY);
	~TileMap();

	void clearMap(bool clearEverything);
	void generateMap(short traversableRatio, short obstacleRatio);
	bool findTile(short &index_X, short &index_Y, short tileType);
	std::vector<Path> findTiles(short tileType);
	ShortNode* getNode(short posX, short posY);
	short getTile(short posX, short posY);
	short getTileType(ShortNode* node);
	void setTileType(ShortNode* node, short value);

	void setTileType(Vector2Int position, short value);
	short getWidth();
	short getHeight();
};