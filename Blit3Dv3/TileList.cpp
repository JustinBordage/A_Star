#include "TileList.h"
#include <cassert>
#include <iostream>

TileNode::TileNode()
{
	data = NULL;
	next = NULL;
}

TileNode::TileNode(Node* _data)
{
	data = _data;
	next = NULL;
}

TileNode::~TileNode()
{
	if (next != NULL)
		delete next;
}

TileList::TileList()
{
	head = NULL;
}

TileList::~TileList()
{
	Destroy();
}

TileNode* TileList::getNode(int index)
{
	if (index >= 0)
	{
		if (head != NULL)
		{
			TileNode* tileNode = head;

			for (int size = 0; size < index; size++)
			{
				if (tileNode->next != NULL)
					tileNode = tileNode->next;
				else
					return NULL;
			}

			return tileNode;
		}
	}

	return NULL;
}

Node* TileList::get(int index)
{
	TileNode* tileNode = getNode(index);

	if (tileNode != NULL)
		return tileNode->data;
	else
		return NULL;
}

Node* TileList::get(Vector2Int position)
{
	TileNode* tileNode = NULL;

	for (int index = 0; index < size(); index++)
	{
		tileNode = getNode(index);

		if (tileNode != NULL)
		{
			if (tileNode->data->tilePos == position)
				break;
		}
		else
			std::cout << "\tTileNode was Null" << std::endl;
	}

	if (tileNode != NULL)
		return tileNode->data;
	else
		return NULL;
}

void TileList::add(Node* node)
{
	if (head != NULL)
	{
		TileNode* tail = head;

		while (tail->next != NULL)
		{
			tail = tail->next;
		}

		tail->next = new TileNode(node);
	}
	else
	{
		head = new TileNode(node);
	}

	listSize = -1;
}

void TileList::addUnique(Node* node)
{
	if (!this->Contains(node))
	{
		this->add(node);
	}
}

void TileList::remove(int _nodeIndex)
{
	if (size() > 0)
	{
		TileNode* _nodeToRemove = NULL;

		if (_nodeIndex != 0)
		{
			TileNode* _node = head;

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

		listSize = -1;
	}
}

void TileList::remove(Node* node)
{
	//Node Deletion is not happening properly

	if (head != NULL)
	{
		int index = 0;
		TileNode* tail = head;

		while (tail != NULL)
		{
			if (tail != NULL)
			{
				//std::cout << "Checking Index: " << index << std::endl;
				//std::cout << "TilePosX: " << tail->tileType->tilePos.x << ", tilePosY: " << tail->tileType->tilePos.y << std::endl;

				if (tail->data->tilePos == node->tilePos)
				{
					remove(index);
					break;
				}
				else
				{
					//std::cout << "Switching Nodes" << std::endl;
					tail = tail->next;
					index++;
				}
			}
		}

		listSize = -1;
	}
}

bool TileList::Contains(Node* node)
{
	TileNode* tail = head;
	Node* currNode;

	while (tail != NULL)
	{
		currNode = tail->data;

		if (currNode->tilePos == node->tilePos)
		{
			//std::cout << "\tThis Pos: " << currNode->tilePos.ToStr() << std::endl;
			//std::cout << "\tOther Pos: " << node->tilePos.ToStr() << std::endl;
			return true;
		}
		else
			tail = tail->next;
	}

	return false;
}

int TileList::size()
{
	if (listSize == -1)
	{
		if (head != NULL)
		{
			TileNode* tail = head;
			listSize = 1;

			while (tail->next != NULL)
			{
				tail = tail->next;
				listSize++;
			}
		}
		else
			listSize = 0;
	}

	return listSize;
}

void TileList::Destroy()
{
	DeleteData();
	Clear();
}

void TileList::DeleteData()
{
	if (head != NULL)
	{
		TileNode* tail = head;

		while (tail != NULL)
		{
			delete tail->data;
			tail = tail->next;
		}
	}
}

void TileList::Clear()
{
	if (head != NULL)
	{
		delete head;
	}

	head = NULL;

	listSize = -1;
} 

bool isBetterNode(Node* currentNode, Node* otherNode)
{
	return (currentNode->fCost() <= otherNode->fCost() || (currentNode->fCost() == otherNode->fCost() && currentNode->hCost < otherNode->hCost));
}

void TileList::Insert(int index, Node* node)
{
	if (head != NULL)
	{
		if (index - 1 >= 0)
		{
			TileNode* parentNode = getNode(index - 1);

			if (parentNode != NULL)
			{
				TileNode* childNode = parentNode->next;

				parentNode->next = new TileNode(node);
				parentNode->next->next = childNode;
			}
			else
				std::cout << "parent is null" << std::endl;
		}
		else
		{
			TileNode* temp = head;
			head = new TileNode(node);
			head->next = temp;
			//std::cout << "Head exists already" << std::endl;
		}
	}
	else
		head = new TileNode(node);

	listSize = -1;
}

void TileList::sortedInsert(Node* node)
{
	if (head != NULL)
	{
		//std::cout << "Head is not Null & stuff" << std::endl;

		int index = 0;
		TileNode* tail = head;

		while (tail != NULL)
		{
			if (isBetterNode(node, tail->data))
			{
				Insert(index, node);
				break;
			}
			else
			{
				index++;
				if (tail->next != NULL)
					tail = tail->next;
				else
				{
					tail->next = new TileNode(node);
					break;
				}
					
			}
		}
	}
	else
	{
		head = new TileNode(node);

		//std::cout << "Head IS Null & stuff" << std::endl;
	}

	listSize = -1;
}

void TileList::printCosts()
{
	size();

	std::cout << "Printing List: " << std::endl;

	Node* nodeToPrint;

	for (int i = 0; i < listSize; i++)
	{
		nodeToPrint = get(i);

		std::cout << "\tIndex #" << i << " - ";
		std::cout << "gCost = " << nodeToPrint->gCost;
		std::cout << ", hCost = " << nodeToPrint->hCost;
		std::cout << ", fCost = " << nodeToPrint->fCost();
		//std::cout << ", Position - (" << nodeToPrint->tilePos << ")" << std::endl;
	}
}