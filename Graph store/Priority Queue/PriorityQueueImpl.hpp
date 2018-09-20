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
	items(std::distance(first, last)) 
{
	directlyInsertAll(first, last);
	buildHeap();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(InputIt first, InputIt last)
{
	auto pos = std::size_t{ 0 };
	for (; first != last; ++first)
	{
		items.insert(ItemAdapter{ *first, pos++ });
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
	invalidateHandlesOfAll(std::is_same<HandleSetter, EmptyFunction<Item>>{});
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll(std::true_type)
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll(std::false_type)
{
	for (auto&& item : items)
	{
		item.invalidateHandle();
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(const Item& item)
{
	auto position = items.getCount();
	items.insert(ItemAdapter{ item, position });
	siftUp(position);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
{
	auto optimal = ItemAdapter{ getOptimal() };
	moveLastToRoot();

	if (!isEmpty())
	{
		siftDown(0);
	}
	
	return std::move(optimal).wrappedItem();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());
	return items[0].wrappedItem();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::moveLastToRoot()
{
	auto lastItemIndex = items.getCount() - 1;
	insertAt(0, std::move(items[lastItemIndex]));
	items.removeAt(lastItemIndex);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const Handle& handle, const Key& key)
{
	//TODO: static_assert for not being called with EmptyFunction as HandleSetter
	assert(handle.isValid());
	assert(hasItemAt(handle));

	auto& item = items[handle];
	assert(compareFunction(item.key(), key));
	item.setKey(key);
	siftUp(handle);
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
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insertAt(std::size_t index, ItemAdapter&& adapter)
{
	assert(hasItemAt(index));
	items[index] = ItemAdapter{ std::move(adapter).wrappedItem(), index };
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
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const ItemAdapter& lhs, const ItemAdapter& rhs)  const
{
	return compareFunction(lhs.key(), rhs.key());
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