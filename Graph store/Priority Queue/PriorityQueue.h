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

	void improveKey(const PriorityQueueHandle& handle, Key&& key);

private:
	void swapContentsWithReconstructedParameter(PriorityQueue temporary);

	void directlyInsertAll(Iterator<Element>& iterator);
	void buildHeapOfCurrentElements();
	void siftDown(size_t index);
	void siftUp(size_t index);

	void setElementAtWith(size_t index, Element&& element);

	size_t getParentIndex(size_t index) const;
	size_t getLeftChildIndex(size_t index) const;
	size_t getOptimalChildIndex(size_t index) const;

	bool hasChildren(size_t index) const;
	bool hasParent(size_t index) const;
	bool hasOptimalRightSibling(size_t index) const;

	bool compare(const Element& lhs, const Element& rhs) const;
	void updateHandleOfElementAt(size_t index);
	void invalidateHandleOfElementAt(size_t index);
	void setHandleOfElementAtWith(size_t index, const PriorityQueueHandle& handle);

	bool hasElementAt(size_t index) const;
	bool hasSpaceInCurrentArray() const;
	static bool validateElement(const Element& element);

private:
	ElementArray elements;
	size_t insertedCount;
	mutable CompareFunction compareFunction;     
	mutable HandleSetter handleSetter;
};

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__