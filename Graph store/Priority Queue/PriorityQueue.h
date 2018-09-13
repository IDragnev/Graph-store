#ifndef __PRIORITY_QUEUE_H_INCLUDED__
#define __PRIORITY_QUEUE_H_INCLUDED__

#include "Priority Queue Handle\PriorityQueueHandle.h"
#include "..\Dynamic Array\DArray.h"

template <typename T>
struct EmptyFunction
{
	void operator()(const T&) const { }
};


template <typename T>
struct IdentityAccessor
{
	const T& operator()(const T& item) const { return item; }
	void operator()(T& item, const T& key) const { item = key; }
};


template <
	typename Item,
	typename Key = Item,
	typename KeyAccessor = IdentityAccessor<Item>,
	typename CompareFunction = std::less<Key>,
	typename HandleSetter = EmptyFunction<Item>
> class PriorityQueue
{
public:
	PriorityQueue() = default;
	template <typename InputIt>
	PriorityQueue(InputIt first, InputIt last);
	PriorityQueue(std::initializer_list<Item> source);
	PriorityQueue(PriorityQueue&& source) = default;
	PriorityQueue(const PriorityQueue& source) = default;
	~PriorityQueue();

	PriorityQueue& operator=(PriorityQueue&& rhs) = default;
	PriorityQueue& operator=(const PriorityQueue& rhs) = default;

	void insert(const Item& item);
	Item extractOptimal();
	const Item getOptimal() const;

	void improveKey(const PriorityQueueHandle& handle, const Key& key);

	bool isEmpty() const;
	void empty();

private:
	template <typename InputIt>
	void directlyInsertAll(InputIt first, InputIt last);
	void buildHeap();
	void siftDown(std::size_t index);
	void siftUp(std::size_t index);
	void insertAt(std::size_t index, Item&& item);
	void moveLastToRoot();

	static bool hasParent(std::size_t index);
	static std::size_t getParentIndex(std::size_t index);
	static std::size_t getLeftChildIndex(std::size_t index);

	bool hasChildren(std::size_t index) const;
	std::size_t getOptimalChildIndex(std::size_t index) const;
	bool hasOptimalRightSibling(std::size_t index) const;
	bool hasSmallerPriorityThan(const Item& lhs, const Item& rhs) const;

	void setKeyOf(Item& item, const Key& key);
	void updateHandleOfItemAt(std::size_t index);
	void invalidateHandlesOfAll();
	void invalidateHandlesOfAll(std::true_type);
	void invalidateHandlesOfAll(std::false_type);
	void invalidateHandleOf(Item& item);
	void setHandleOf(Item& item, const PriorityQueueHandle& handle);

	bool hasItemAt(std::size_t index) const;

private:
	DArray<Item> items;
	mutable KeyAccessor keyAccessor;
	mutable CompareFunction compareFunction;     
	HandleSetter handleSetter;
};

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__