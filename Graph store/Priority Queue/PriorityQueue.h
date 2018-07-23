#ifndef __PRIORITY_QUEUE_H_INCLUDED__
#define __PRIORITY_QUEUE_H_INCLUDED__

#include "Pair\Pair.h"
#include "Priority Queue Handle\PriorityQueueHandle.h"
#include "../Dynamic Array/DArray.h"

#include "../Iterator abstraction/Iterator.h"

template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
class PriorityQueue
{
private:
	typedef Pair<Item, Key> Element;
	typedef DArray<Element> ElementArray;

public:
	PriorityQueue();
	PriorityQueue(Iterator<Element>& iterator, size_t count);
	PriorityQueue(PriorityQueue&& source);
	PriorityQueue(const PriorityQueue& source) = default;
	~PriorityQueue() = default;

	PriorityQueue& operator=(PriorityQueue&& rhs);
	PriorityQueue& operator=(const PriorityQueue& rhs) = default;

	bool isEmpty() const;

	void insert(Element&& newElement);
	Item& extractOptimal();
	const Item& getOptimal() const;

	void updateKey(PriorityQueueHandle handle, Key&& key);

private:
	ElementArray elements;
	size_t insertedCount;
	mutable CompareFunction compare;     
	mutable HandleSetter handleSetter;
};

#endif //__PRIORITY_QUEUE_H_INCLUDED__