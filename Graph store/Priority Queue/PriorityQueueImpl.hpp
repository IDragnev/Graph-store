#include <assert.h>
#include <algorithm>

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&& source) :
	items{ std::move(source.items) },
	insertedCount{ source.insertedCount },
	keyAccessor{ std::move(source.keyAccessor) },
	compareFunction{ std::move(source.compareFunction) },
	handleSetter{ std::move(source.handleSetter) }
{
	source.insertedCount = 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::operator=(PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWithReconstructedParameter(std::move(rhs));
	}

	return *this;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::swapContentsWithReconstructedParameter(PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter> temporary)
{
	std::swap(items, temporary.items);
	std::swap(insertedCount, temporary.insertedCount);
	std::swap(keyAccessor, temporary.keyAccessor);
	std::swap(compareFunction, temporary.compareFunction);
	std::swap(handleSetter, temporary.handleSetter);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(InputIt first, InputIt last)
{
	directlyInsertAll(first, last);
	buildHeap();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(InputIt first, InputIt last)
{

	std::for_each(first, last, [&](Item* item)
	{
		items.insert(item);
		++insertedCount;
	});

	assert(!hasSpaceInCurrentArray());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::buildHeap()
{
	for (long lastNonLeaf = (insertedCount / 2) - 1; lastNonLeaf >= 0; --lastNonLeaf)
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
		invalidateHandleOf(item);
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(Item* newItem)
{
	assert(newItem);

	if (hasSpaceInCurrentArray())
	{
		items[insertedCount] = newItem;
	}
	else
	{
		items.insert(newItem);
	}

	updateHandleOfItemAt(insertedCount++);
	siftUp(insertedCount - 1);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
Item* PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
{
	auto optimal = const_cast<Item*>(getOptimal());
	invalidateHandleOf(optimal);

	--insertedCount;

	if (!isEmpty())
	{
		insertAt(0, items[insertedCount]);
		siftDown(0);
	}

	return optimal;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item* PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());
	return items[0];
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const PriorityQueueHandle& handle, const Key& key)
{
	assert(handle.isValid());
	assert(hasItemAt(handle));

	auto& itemToImprove = *items[handle];
	assert(compareFunction(keyAccessor.getKeyOf(itemToImprove), key));

	keyAccessor.setKeyOf(itemToImprove, key);
	siftUp(handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(std::size_t index)
{
	Item* itemToSift = items[index];

	while (hasParent(index))
	{
		std::size_t parentIndex = getParentIndex(index);
		Item* parent = items[parentIndex];

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
	Item* itemToSift = items[index];

	while (hasChildren(index))
	{
		std::size_t optimalChildIndex = getOptimalChildIndex(index);
		Item* optimalChild = items[optimalChildIndex];

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
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insertAt(std::size_t index, Item* item)
{
	assert(hasItemAt(index));
	assert(item);

	items[index] = item;
	updateHandleOfItemAt(index);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandleOfItemAt(std::size_t index)
{
	setHandleOf(items[index], PriorityQueueHandle(index));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandleOf(Item* item)
{
	setHandleOf(item, PriorityQueueHandle::invalidHandle());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::setHandleOf(Item* item, const PriorityQueueHandle& handle)
{
	assert(item);
	handleSetter(*item, handle);
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
	std::size_t result = getLeftChildIndex(index);

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
	return hasItemAt(leftChildIndex + 1) && hasSmallerPriorityThan(items[leftChildIndex], items[leftChildIndex + 1]);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const Item* lhs, const Item* rhs)  const
{
	return compareFunction(keyAccessor.getKeyOf(*lhs), keyAccessor.getKeyOf(*rhs));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSpaceInCurrentArray() const
{
	return insertedCount < items.getCount();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasItemAt(std::size_t index) const
{
	return index < insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::empty()
{
	invalidateHandlesOfAllItems();
	items.empty();
	insertedCount = 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const
{
	return insertedCount == 0;
}