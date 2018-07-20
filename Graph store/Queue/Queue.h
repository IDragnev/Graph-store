#ifndef __SINGLY_LINKED_LIST_ADAPTER_QUEUE_H_INCLUDED__
#define __SINGLY_LINKED_LIST_ADAPTER_QUEUE_H_INCLUDED__

#include "../Singly Linked List/SinglyLinkedList.h"

template <typename T>
class Queue
{
public:
	void enqueue(const T& item);
	T dequeue();
	T peekHead() const;
	
	bool isEmpty() const;
	void empty();

private:
	SinglyLinkedList<T> theUnderlyingList;
};

#include "QueueImpl.hpp"
#endif //__SINGLY_LINKED_LIST_ADAPTER_QUEUE_H_INCLUDED__