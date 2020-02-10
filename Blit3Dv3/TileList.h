#pragma once
#include "Node.h"
#include "TileMap.h"

class TileNode
{
public:
	TileNode* next;
	Node* data;

	TileNode();
	TileNode(Node* _data);
	~TileNode();
};

class TileList
{
private:
	int listSize = -1;

	TileNode* getNode(int index);

public:
	TileNode* head;

	TileList();
	~TileList();
	Node* get(int index);
	Node* get(Vector2Int position);

	void Insert(int index, Node* node);
	void sortedInsert(Node* node);
	void add(Node* node);
	void addUnique(Node* node);
	void remove(int _nodeIndex);
	void remove(Node* node);
	bool Contains(Node* node);
	int size();
	void Destroy();
	void Clear();
	void DeleteData();
	void printCosts();

	Node* operator [](const int& index)
	{
		return get(index);
	}
};

