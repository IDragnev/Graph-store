#include <assert.h>
#include <algorithm>


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(InputIt first, InputIt last) :
	items{ std::distance(first, last) }
{
	directlyInsertAll(first, last);
	buildHeap();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(InputIt first, InputIt last)
{
	auto position = std::size_t{ 0 };
	std::for_each(first, last, [&](Item& item) 
	{ 
		items.insert(&item);
		updateHandleOfItemAt(position++);
	});
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::buildHeap()
{
	for (long lastNonLeaf = (items.getCount() / 2) - 1; lastNonLeaf >= 0; --lastNonLeaf)
	{
		siftDown(lastNonLeaf);
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::~PriorityQueue()
{
	invalidateHandlesOfAllItems();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAllItems()
{
	for (auto&& item : items)
	{
		invalidateHandleOf(*item);
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(Item& newItem)
{
	auto position = items.getCount();
	items.insert(&newItem);
	updateHandleOfItemAt(position);
	siftUp(position);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
Item& PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
{
	auto& optimal = const_cast<Item&>(getOptimal());
	invalidateHandleOf(optimal);

	auto count = items.getCount();
	if (count > 1)
	{
		insertAt(0, itemAt(count - 1));
		items.removeAt(count - 1);
		siftDown(0);
	}
	else
	{
		items.removeAt(count - 1); //TODO: should it be in an else clause?
	}

	return optimal;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item& PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());
	return itemAt(0);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const PriorityQueueHandle& handle, const Key& key)
{
	assert(handle.isValid());
	assert(hasItemAt(handle));

	auto& item = itemAt(handle);
	assert(compareFunction(keyAccessor(item), key));
	keyAccessor(item, key); //TODO: another parameter or just a named inline function
	siftUp(handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(std::size_t index)
{
	auto& itemToSift = itemAt(index);

	while (hasParent(index))
	{
		auto parentIndex = getParentIndex(index);
		auto& parent = itemAt(parentIndex);

		if (hasSmallerPriorityThan(parent, itemToSift))
		{
			insertAt(index, parent);
			index = parentIndex;
		}
		else
		{
			break;
		}
	}

	insertAt(index, itemToSift);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftDown(std::size_t index)
{
	auto& itemToSift = itemAt(index);

	while (hasChildren(index))
	{
		auto optimalChildIndex = getOptimalChildIndex(index);
		auto& optimalChild = itemAt(optimalChildIndex);

		if (hasSmallerPriorityThan(itemToSift, optimalChild))
		{
			insertAt(index, optimalChild);
			index = optimalChildIndex;
		}
		else 
		{
			break;
		}
	}

	insertAt(index, itemToSift);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insertAt(std::size_t index, Item& item)
{
	assert(hasItemAt(index));
	items[index] = &item;
	updateHandleOfItemAt(index);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandleOfItemAt(std::size_t index)
{
	setHandleOf(itemAt(index), PriorityQueueHandle(index));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandleOf(Item& item)
{
	setHandleOf(item, PriorityQueueHandle::invalidHandle());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::setHandleOf(Item& item, const PriorityQueueHandle& handle)
{
	handleSetter(item, handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasParent(std::size_t index)
{
	return index > 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getParentIndex(std::size_t index)
{
	return (index - 1) / 2;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasChildren(std::size_t index) const
{
	return hasItemAt(getLeftChildIndex(index));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimalChildIndex(std::size_t index) const
{
	auto result = getLeftChildIndex(index);

	if (hasOptimalRightSibling(result))
	{
		++result;
	}

	return result;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getLeftChildIndex(std::size_t index)
{
	return 2 * index + 1;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasOptimalRightSibling(std::size_t leftChildIndex) const
{
	return hasItemAt(leftChildIndex + 1) && 
		   hasSmallerPriorityThan(itemAt(leftChildIndex), itemAt(leftChildIndex + 1));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const Item& lhs, const Item& rhs)  const
{
	return compareFunction(keyAccessor(lhs), keyAccessor(rhs));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasItemAt(std::size_t index) const
{
	return index < items.getCount();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::empty()
{
	invalidateHandlesOfAllItems();
	items.empty();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const
{
	return items.isEmpty();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline Item& PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::itemAt(std::size_t index)
{
	return const_cast<Item&>( static_cast<const PriorityQueue&>(*this).itemAt(index) );
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item& PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::itemAt(std::size_t index) const
{
	return *items[index];
}
