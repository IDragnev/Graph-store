#ifndef __SINGLY_LINKED_LIST_ADAPTER_QUEUE_H_INCLUDED__
#define __SINGLY_LINKED_LIST_ADAPTER_QUEUE_H_INCLUDED__

#include "../Singly Linked List/SinglyLinkedList.h"

template <typename T>
class Queue
{
public:
	bool isEmpty() const;
	void empty();

	T peekHead() const;
	T dequeue();
	void enqueue(const T& item);

private:
	SinglyLinkedList<T> theUnderlyingList;
};

#include "QueueImpl.hpp"
#endif //__SINGLY_LINKED_LIST_ADAPTER_QUEUE_H_INCLUDED__