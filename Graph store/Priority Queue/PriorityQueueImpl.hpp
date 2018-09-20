#include <assert.h>
#include <algorithm>

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
HandleSetter PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::ItemAdapter::handleSetter;

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
KeyAccessor PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::ItemAdapter::keyAccessor;


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(std::initializer_list<Item> source) :
	PriorityQueue{ source.begin(), source.end() }
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(InputIt first, InputIt last) :
	items(first, last) 
{
	updateHandlesOfAll();
	buildHeap();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandlesOfAll()
{
	updateHandlesOfAll(should_set_handles_t{});
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandlesOfAll(do_not_set_handles)
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandlesOfAll(set_handles)
{
	auto count = items.getCount();
	auto pos = 0U;
	while (pos < count)
	{
		updateHandleOfItemAt(pos++);
	}
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
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::~PriorityQueue()
{
	invalidateHandlesOfAll();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll()
{
	invalidateHandlesOfAll(should_set_handles_t{});
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll(do_not_set_handles)
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll(set_handles)
{
	for (auto&& item : items)
	{
		invalidateHandleOf(item);
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(const Item& item)
{
	auto position = items.getCount();
	items.insert(item);
	updateHandleOfItemAt(position);
	siftUp(position);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
{
	auto optimal = getOptimal();
	invalidateHandleOf(optimal);

	moveLastToRoot();

	if (!isEmpty())
	{
		siftDown(0);
	}
	
	return optimal;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::moveLastToRoot()
{
	auto count = items.getCount();
	insertAt(0, std::move(items[--count]));
	items.removeAt(count);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());
	return items[0];
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const PriorityQueueHandle& handle, const Key& key)
{
	assert(handle.isValid());
	assert(hasItemAt(handle));

	auto& item = items[handle];
	assert(compareFunction(keyAccessor(item), key));
	setKeyOf(item, key);
	siftUp(handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::setKeyOf(Item& item, const Key& key)
{
	keyAccessor(item, key);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(std::size_t index)
{
	auto itemToSift = std::move(items[index]);

	while (hasParent(index))
	{
		auto parentIndex = getParentIndex(index);
		auto& parent = items[parentIndex];

		if (hasSmallerPriorityThan(parent, itemToSift))
		{
			insertAt(index, std::move(parent));
			index = parentIndex;
		}
		else
		{
			break;
		}
	}

	insertAt(index, std::move(itemToSift));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftDown(std::size_t index)
{
	auto itemToSift = std::move(items[index]);

	while (hasChildren(index))
	{
		auto optimalChildIndex = getOptimalChildIndex(index);
		auto& optimalChild = items[optimalChildIndex];

		if (hasSmallerPriorityThan(itemToSift, optimalChild))
		{
			insertAt(index, std::move(optimalChild));
			index = optimalChildIndex;
		}
		else 
		{
			break;
		}
	}

	insertAt(index, std::move(itemToSift));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insertAt(std::size_t index, Item&& item)
{
	assert(hasItemAt(index));
	items[index] = std::move(item);
	updateHandleOfItemAt(index);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandleOfItemAt(std::size_t index)
{
	setHandleOf(items[index], index);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandleOf(Item& item)
{
	setHandleOf(item, PriorityQueueHandle{});
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
		   hasSmallerPriorityThan(items[leftChildIndex], items[leftChildIndex + 1]);
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
	invalidateHandlesOfAll();
	items.empty();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const
{
	return items.isEmpty();
}