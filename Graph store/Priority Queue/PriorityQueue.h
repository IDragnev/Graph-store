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
	struct set_handles { };
	struct do_not_set_handles { };
	struct choose_handle_setting_behaviour
	{
		static constexpr bool shouldSet = !std::is_same<HandleSetter, EmptyFunction<Item>>::value;
		using type = std::conditional_t<shouldSet, set_handles, do_not_set_handles>;
	};

	using should_set_handles_t = typename choose_handle_setting_behaviour::type;
	using Handle = PriorityQueueHandle;

	class ItemAdapter
	{
	public:
		ItemAdapter() = default;
		ItemAdapter(ItemAdapter&& source) = default;
		ItemAdapter(const ItemAdapter& source) = default;
		ItemAdapter(Item&& item, const Handle& h = {}) : item{ std::move(item) } { setHandle(h); }
		ItemAdapter(const Item& item, const Handle& h = {}) : item{ item } { setHandle(h); }

		ItemAdapter& operator=(ItemAdapter&& rhs) = default;
		ItemAdapter& operator=(const ItemAdapter& rhs) = default;

		const Key& key() const { return keyAccessor(item); }
		void setKey(const Key& key) { keyAccessor(item, key); }

		const Item& wrappedItem() const & { return item; }
		Item wrappedItem() && { return std::move(item); }
		Item releaseWrapped() && 
		{ 
			invalidateHandle(); 
			return std::move(item);
		}

		void invalidateHandle() { setHandle(Handle{}); }
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
	void updateHandlesOfAll();
	void updateHandlesOfAll(set_handles);
	void updateHandlesOfAll(do_not_set_handles);
	void updateHandleOfItemAt(std::size_t index);
	void invalidateHandlesOfAll();
	void invalidateHandlesOfAll(set_handles);
	void invalidateHandlesOfAll(do_not_set_handles);
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