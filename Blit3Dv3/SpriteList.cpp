#include "SpriteList.h"
#include <iostream>
#include <Blit3D.h>

SpriteNode::SpriteNode(Sprite* sprite)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	SpriteNode
	//
	//Method parameters	:	Sprite* sprite
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructor for the spriteNode
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	spriteData = sprite;
	next = NULL;
}

SpriteNode::~SpriteNode()
{
	if (next != NULL)
		delete next;
}

SpriteList::SpriteList()
{
	head = NULL;
}

SpriteList::~SpriteList()
{
	clear();
}

Sprite* SpriteList::get(int index)
{
	if (head != NULL)
	{
		SpriteNode* spriteNode = head;

		for (int size = 0; size < index; size++)
		{
			if (spriteNode->next != NULL)
				spriteNode = spriteNode->next;
			else
				return NULL;
		}

		return spriteNode->spriteData;
	}
	else
		return NULL;
}

void SpriteList::add(Sprite* sprite)
{
	if (head != NULL)
	{
		SpriteNode* spriteNode = head;

		while (spriteNode->next != NULL)
		{
			spriteNode = spriteNode->next;
		}

		spriteNode->next = new SpriteNode(sprite);
	}
	else
	{
		head = new SpriteNode(sprite);
	}
}

void SpriteList::remove(int _nodeIndex)
{
	if (size() > 0)
	{
		SpriteNode* _nodeToRemove = NULL;

		if (_nodeIndex != 0)
		{
			SpriteNode* _node = head;

			//Cycles through the list until having reached
			//the node before the node that is to be removed
			for (int index = 0; index < _nodeIndex - 1; index++)
			{
				if (_node->next != NULL)
					_node = _node->next;
				else
					assert(_node->next != NULL);
			}

			//Retrieves the node to be removed if it exists
			if (_node->next != NULL)
				_nodeToRemove = _node->next;
			else
				assert(_node->next != NULL);

			//Rebinds the next pointer (doesn't matter if it's NULL)
			_node->next = _nodeToRemove->next;
		}
		else
		{
			//Sets the head to the nodeToRemove
			//The safety check shouldn't be required
			if (head != NULL)
				_nodeToRemove = head;
			else
				assert(head != NULL);

			//Rebinds the head pointer (doesn't matter if it's NULL)
			head = _nodeToRemove->next;
		}

		//Sets the next pointer of the nodeToRemove to NULL so
		//the rest of the list (if it exists) doesn't get deleted
		_nodeToRemove->next = NULL;

		//Deletes the node at the given index
		delete _nodeToRemove;
	}
}

int SpriteList::size()
{
	if (head != NULL)
	{
		SpriteNode* tail = head;
		int sizeCounter = 1;

		while (tail->next != NULL)
		{
			tail = tail->next;
			sizeCounter++;
		}

		return sizeCounter;
	}
	else
		return 0;

}

void SpriteList::clear()
{
	if (head != NULL)
	{
		delete head;
	}

	head = NULL;
}