#ifndef __PRIORITY_QUEUE_H_INCLUDED__
#define __PRIORITY_QUEUE_H_INCLUDED__

#include "Priority Queue Handle\PriorityQueueHandle.h"
#include "../Dynamic Array/DArray.h"
#include "../Iterator abstraction/Iterator.h"

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
class PriorityQueue
{
public:
	PriorityQueue();
	PriorityQueue(Iterator<Item>& iterator, size_t count);
	PriorityQueue(PriorityQueue&& source);
	PriorityQueue(const PriorityQueue& source) = default;
	~PriorityQueue() = default;

	PriorityQueue& operator=(PriorityQueue&& rhs);
	PriorityQueue& operator=(const PriorityQueue& rhs) = default;

	bool isEmpty() const;

	void insert(Item& item);
	Item& extractOptimal();
	const Item& getOptimal() const;

	void improveKey(const PriorityQueueHandle& handle, const Key& key);

private:
	void swapContentsWithReconstructedParameter(PriorityQueue temporary);

	void directlyInsertAll(Iterator<Item>& iterator);
	void buildHeap();
	void siftDown(size_t index);
	void siftUp(size_t index);

	void insertAt(size_t index, Item& item);

	static bool hasParent(size_t index);
	static size_t getParentIndex(size_t index);
	static size_t getLeftChildIndex(size_t index);

	bool hasChildren(size_t index) const;
	size_t getOptimalChildIndex(size_t index) const;
	bool hasOptimalRightSibling(size_t index) const;

	bool hasSmallerPriorityThan(const Item& lhs, const Item& rhs) const;
	void updateHandleOfItemAt(size_t index);
	void invalidateHandleOfItemAt(size_t index);
	void setHandleOfItemAtWith(size_t index, const PriorityQueueHandle& handle);

	Item& getItemAt(size_t index);
	const Item& getItemAt(size_t index) const;
	bool hasItemAt(size_t index) const;
	bool hasSpaceInCurrentArray() const;

private:
	DArray<Item*> items;
	size_t insertedCount;
	mutable KeyAccessor keyAccessor;
	mutable CompareFunction compareFunction;     
	HandleSetter handleSetter;
};

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__