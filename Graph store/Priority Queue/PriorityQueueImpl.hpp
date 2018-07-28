#include <assert.h>


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue() :
	items(),
	insertedCount(0)
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&& source) :
	items(std::move(source.items)),
	insertedCount(source.insertedCount),
	keyAccessor(std::move(source.keyAccessor)),
	compareFunction(std::move(source.compareFunction)),
	handleSetter(std::move(source.handleSetter))
{
	source.insertedCount = 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&
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
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(Iterator<Item*>& iterator, size_t count) :
	items(count, count),
	insertedCount(0)
{
	directlyInsertAll(iterator);
	buildHeap();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(Iterator<Item*>& iterator)
{
	assert(isEmpty());

	while (!iterator.isFinished())
	{
		assert(hasSpaceInCurrentArray());
		insertAt(insertedCount++, iterator.getCurrent());

		iterator.goToNext();
	}

	assert(!hasSpaceInCurrentArray());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::buildHeap()
{
	for (long lastNonLeaf = (insertedCount / 2) - 1; lastNonLeaf >= 0; --lastNonLeaf)
	{
		siftDown(lastNonLeaf);
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
	Item* optimal = const_cast<Item*>(getOptimal());
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

	Item& itemToImprove = *items[handle];
	assert(compareFunction(keyAccessor.getKey(itemToImprove), key));

	keyAccessor.setKey(itemToImprove, key);
	siftUp(handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(size_t index)
{
	Item* itemToSift = items[index];

	while (hasParent(index))
	{
		size_t parentIndex = getParentIndex(index);
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
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftDown(size_t index)
{
	Item* itemToSift = items[index];

	while (hasChildren(index))
	{
		size_t optimalChildIndex = getOptimalChildIndex(index);
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
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insertAt(size_t index, Item* item)
{
	assert(hasItemAt(index));
	assert(item);

	items[index] = item;
	updateHandleOfItemAt(index);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandleOfItemAt(size_t index)
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
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasParent(size_t index)
{
	return index > 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getParentIndex(size_t index)
{
	return (index - 1) / 2;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasChildren(size_t index) const
{
	return hasItemAt(getLeftChildIndex(index));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimalChildIndex(size_t index) const
{
	size_t result = getLeftChildIndex(index);

	if (hasOptimalRightSibling(result))
	{
		++result;
	}

	return result;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getLeftChildIndex(size_t index)
{
	return 2 * index + 1;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasOptimalRightSibling(size_t leftChildIndex) const
{
	return hasItemAt(leftChildIndex + 1) && hasSmallerPriorityThan(items[leftChildIndex], items[leftChildIndex + 1]);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const Item* lhs, const Item* rhs)  const
{
	return compareFunction(keyAccessor.getKey(*lhs), keyAccessor.getKey(*rhs));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSpaceInCurrentArray() const
{
	return insertedCount < items.getCount();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasItemAt(size_t index) const
{
	return index < insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const
{
	return insertedCount == 0;
}
