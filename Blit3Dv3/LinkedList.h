#pragma once
#include <Blit3D.h>



template <typename Type>
class ListNode
{
public:
	ListNode* next;
	Type data;

	ListNode(Type _data)
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

	~ListNode()
	{
		if (next != NULL)
			delete next;
	}
};

template <typename Type>
class LinkedList
{
private:
	int ListSize = -1;
	ListNode<Type>* head;

public:
	//Constructs the list object
	LinkedList()
	{
		head = NULL;
	}

	//Destroys the list, without destroying the data
	~LinkedList()
	{
		Clear();
	}

	//Retrieves the data at the index
	Type get(int index)
	{
		if (head != NULL)
		{
			ListNode<Type>* tail = head;

			for (int size = 0; size < index; size++)
			{
				if (tail->next != NULL)
					tail = tail->next;
				else
					return Type();
			}

			return tail->data;
		}
		else
			return Type();
	}

	//Adds data to the end of the list
	void add(Type _data)
	{
		if (head != NULL)
		{
			ListNode<Type>* tail = head;

			while (tail->next != NULL)
			{
				tail = tail->next;
			}

			tail->next = new ListNode<Type>(_data);
		}
		else
		{
			head = new ListNode<Type>(_data);
		}
	}

	//Removes the list data at the index
	void remove(int _nodeIndex)
	{
		if (size() > 0)
		{
			ListNode<Type>* _nodeToRemove = NULL;

			if (_nodeIndex != 0)
			{
				ListNode<Type>* _node = head;

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

	//Returns the size of the list
	int size()
	{
		if (ListSize == -1)
		{
			if (head != NULL)
			{
				ListNode<Type>* tail = head;
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

	//Clears the indexes but leaves the data intact
	void Clear()
	{
		if (head != NULL)
		{
			delete head;
		}

		head = NULL;
	}
	
	/*
	template <typename Type>
	//Clears the Indexes and the Data
	void Destroy()
	{
		if (head != NULL)
		{
			delete head;
		}
	
		head = NULL;
	}*/
};