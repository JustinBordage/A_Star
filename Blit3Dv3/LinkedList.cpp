#include "LinkedList.h"
#include <iostream>
#include <Blit3D.h>



template <typename Type>
ListNode<Type>::ListNode(Type _data)
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	ListNode
	//
	//Method parameters	:	Type* data
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructor for the ListNode
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/16/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	data = _data;
	next = NULL;
}

template <typename Type>
ListNode<Type>::~ListNode()
{
	if (next != NULL)
		delete next;
}

template <typename Type>
LinkedList<Type>::LinkedList()
{
	head = NULL;
}

template <typename Type>
LinkedList<Type>::~LinkedList()
{
	Clear();
}

template <typename Type>
Type LinkedList<Type>::get(int index)
{
	if (head != NULL)
	{
		NodeLink* dataNode = head;

		for (int size = 0; size < index; size++)
		{
			if (dataNode->next != NULL)
				dataNode = dataNode->next;
			else
				return NULL;
		}

		return dataNode->spriteData;
	}
	else
		return NULL;
}

template <typename Type>
void LinkedList<Type>::add(Type sprite)
{
	if (head != NULL)
	{
		NodeLink* dataNode = head;

		while (dataNode->next != NULL)
		{
			dataNode = dataNode->next;
		}

		dataNode->next = new NodeLink(sprite);
	}
	else
	{
		head = new NodeLink(sprite);
	}
}

template <typename Type>
void LinkedList<Type>::remove(int _nodeIndex)
{
	if (size() > 0)
	{
		NodeLink* _nodeToRemove = NULL;

		if (_nodeIndex != 0)
		{
			NodeLink* _node = head;

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

template <typename Type>
int LinkedList<Type>::size()
{
	if (ListSize == -1)
	{
		if (head != NULL)
		{
			NodeLink* tail = head;
			ListSize = 1;

			while (tail->next != NULL)
			{
				tail = tail->next;
				ListSize++;
			}
		}
		else
			ListSize = 0;
	}

	return ListSize;
}

template <typename Type>
void LinkedList<Type>::Clear()
{
	if (head != NULL)
	{
		delete head;
	}

	head = NULL;
}

/*template <typename Type>
void LinkedList<Type>::Destroy()
{
	if (head != NULL)
	{
		delete head;
	}

	head = NULL;
}*/