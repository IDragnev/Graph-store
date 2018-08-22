#ifndef __PRIORITY_QUEUE_H_INCLUDED__
#define __PRIORITY_QUEUE_H_INCLUDED__

#include "Priority Queue Handle\PriorityQueueHandle.h"
#include "..\Dynamic Array\DArray.h"
#include "..\Iterator abstraction\ConstIterator.h"

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
class PriorityQueue
{
public:
	PriorityQueue() = default;
	PriorityQueue(ConstIterator<Item*>& iterator, std::size_t count);
	PriorityQueue(PriorityQueue&& source);
	PriorityQueue(const PriorityQueue& source) = default;
	~PriorityQueue();

	PriorityQueue& operator=(PriorityQueue&& rhs);
	PriorityQueue& operator=(const PriorityQueue& rhs) = default;

	void insert(Item* item);
	Item* extractOptimal();
	const Item* getOptimal() const;

	void improveKey(const PriorityQueueHandle& handle, const Key& key);

	bool isEmpty() const;
	void empty();

private:
	void swapContentsWithReconstructedParameter(PriorityQueue temporary);

	void directlyInsertAll(ConstIterator<Item*>& iterator);
	void buildHeap();
	void siftDown(std::size_t index);
	void siftUp(std::size_t index);
	void insertAt(std::size_t index, Item* item);

	static bool hasParent(std::size_t index);
	static std::size_t getParentIndex(std::size_t index);
	static std::size_t getLeftChildIndex(std::size_t index);

	bool hasChildren(std::size_t index) const;
	std::size_t getOptimalChildIndex(std::size_t index) const;
	bool hasOptimalRightSibling(std::size_t index) const;
	bool hasSmallerPriorityThan(const Item* lhs, const Item* rhs) const;

	void updateHandleOfItemAt(std::size_t index);
	void invalidateHandlesOfAllItems();
	void invalidateHandleOf(Item* item);
	void setHandleOf(Item* item, const PriorityQueueHandle& handle);

	bool hasItemAt(std::size_t index) const;
	bool hasSpaceInCurrentArray() const;

private:
	DArray<Item*> items;
	std::size_t insertedCount{};
	mutable KeyAccessor keyAccessor;
	mutable CompareFunction compareFunction;     
	HandleSetter handleSetter;
};

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__