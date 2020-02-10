#pragma once
#include <Blit3D.h>

class DataNode
{
public:
	DataNode* next;
	Sprite* spriteData;

	DataNode(Sprite* sprite);
	~DataNode();
};

class LinkedList
{
private:
	DataNode* head;

public:
	LinkedList();
	~LinkedList();
	Sprite* get(int index);
	void add(Sprite* sprite);
	void remove(int _nodeIndex);
	int size();
	void clear();
};