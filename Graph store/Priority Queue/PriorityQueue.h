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
private:
	using Handle = PriorityQueueHandle;

	class ItemAdapter
	{
	public:
		ItemAdapter() = default;
		ItemAdapter(Item&& item, const Handle& h = {}) : item{ std::move(item) } { setHandle(h); }
		ItemAdapter(const Item& item, const Handle& h = {}) : item{ item } { setHandle(h); }

		const Key& key() const { return keyAccessor(item); }
		void setKey(const Key& key) { keyAccessor(item, key); }

		const Item& wrappedItem() const & { return item; }
		Item wrappedItem() && { return std::move(item); }

		void invalidateHandle() { setHandle(Handle{}); }

	private:
		void setHandle(const Handle& h) { handleSetter(item, h); }

	private:
		static HandleSetter handleSetter;
		static KeyAccessor keyAccessor;

	private:
		Item item{};
	};

public:
	PriorityQueue() = default;
	template <typename InputIt>
	PriorityQueue(InputIt first, InputIt last);
	PriorityQueue(std::initializer_list<Item> source);
	PriorityQueue(PriorityQueue&& source) = default;
	PriorityQueue(const PriorityQueue& source);
	~PriorityQueue();

	PriorityQueue& operator=(PriorityQueue&& rhs);
	PriorityQueue& operator=(const PriorityQueue& rhs);

	void insert(const Item& item);
	Item extractOptimal();
	const Item getOptimal() const;

	void improveKey(const Handle& handle, const Key& key);

	bool isEmpty() const;
	void empty();

private:
	template <typename InputIt>
	void directlyInsertAll(InputIt first, InputIt last);
	void buildHeap();
	void siftDown(std::size_t index);
	void siftUp(std::size_t index);
	void moveAt(std::size_t index, ItemAdapter&& adapter);
	void moveLastToRoot();

	static bool hasParent(std::size_t index);
	static std::size_t getParentIndex(std::size_t index);
	static std::size_t getLeftChildIndex(std::size_t index);

	bool hasChildren(std::size_t index) const;
	std::size_t getOptimalChildIndex(std::size_t index) const;
	bool hasOptimalRightSibling(std::size_t index) const;
	bool hasSmallerPriorityThan(const ItemAdapter& lhs, const ItemAdapter& rhs) const;
	bool hasAdapterAt(std::size_t index) const;

	void invalidateHandlesOfAll();
	void invalidateHandlesOfAll(std::true_type);
	void invalidateHandlesOfAll(std::false_type);

private:
	DArray<ItemAdapter> adapters;
	mutable CompareFunction compareFunction;     
};

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__