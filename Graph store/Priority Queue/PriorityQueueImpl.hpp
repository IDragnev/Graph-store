#include <assert.h>


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue() :
	elements(),
	insertedCount(0)
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&& source) :
	elements(std::move(source.elements)),
	insertedCount(source.insertedCount),
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
	std::swap(elements, temporary.elements);
	std::swap(insertedCount, temporary.insertedCount);
	std::swap(compareFunction, temporary.compareFunction);
	std::swap(handleSetter, temporary.handleSetter);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(Iterator<Element>& iterator, size_t count) :
	elements(count, count),
	insertedCount(0)
{
	directlyInsertAll(iterator);
	buildHeapOfCurrentElements();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(Iterator<Element>& iterator)
{
	assert(isEmpty());

	while (!iterator.isFinished())
	{
		Element& element = iterator.getCurrent();
		assert(validateElement(element));

		assert(hasSpaceInCurrentArray());
		setElementAtWith(insertedCount++, std::move(element));

		iterator.goToNext();
	}

	assert(!hasSpaceInCurrentArray());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::buildHeapOfCurrentElements()
{
	for (long lastNonLeaf = (insertedCount / 2) - 1; lastNonLeaf >= 0; --lastNonLeaf)
	{
		siftDown(lastNonLeaf);
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(Element&& newElement)
{
	assert(validateElement(newElement));

	if (hasSpaceInCurrentArray())
	{
		elements[insertedCount] = std::move(newElement);
	}
	else
	{
		elements.insert(newElement);
	}

	updateHandleOfElementAt(insertedCount++);
	siftUp(insertedCount - 1);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
Item& PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
{
	assert(!isEmpty());

	invalidateHandleOfElementAt(0);
	Item& optimalItem = const_cast<Item&>(getOptimal());

	--insertedCount;

	if (!isEmpty())
	{
		setElementAtWith(0, std::move(elements[insertedCount]));
		siftDown(0);
	}

	return optimalItem;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item& PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());

	return *(elements[0].itemPtr);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const PriorityQueueHandle& handle, Key&& key)
{
	assert(handle.isValid());
	assert(hasElementAt(handle));
	assert(compareFunction(keyAccessor.getKey(elements[handle]), key));

	keyAccessor.setKey(elements[handle], std::move(key));
	siftUp(handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(size_t index)
{
	Element elementToSift = std::move(elements[index]);

	while (hasParent(index))
	{
		size_t parentIndex = getParentIndex(index);

		if (hasSmallerPriorityThan(elements[parentIndex], elementToSift))  
		{
			setElementAtWith(index, std::move(elements[parentIndex]));
			index = parentIndex;
		}
		else
		{
			break;
		}
	}

	setElementAtWith(index, std::move(elementToSift));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftDown(size_t index)
{
	Element elementToSift = std::move(elements[index]);

	while (hasChildren(index))
	{
		size_t optimalChildIndex = getOptimalChildIndex(index);

		if (hasSmallerPriorityThan(elementToSift, elements[optimalChildIndex])) 
		{
			setElementAtWith(index, std::move(elements[optimalChildIndex]));
			index = optimalChildIndex;
		}
		else 
		{
			break;
		}
	}

	setElementAtWith(index, std::move(elementToSift));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::setElementAtWith(size_t index, Element&& element)
{
	assert(hasElementAt(index));

	elements[index] = std::move(element);
	updateHandleOfElementAt(index);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::updateHandleOfElementAt(size_t index)
{
	setHandleOfElementAtWith(index, PriorityQueueHandle(index));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandleOfElementAt(size_t index)
{
	setHandleOfElementAtWith(index, PriorityQueueHandle::invalidHandle());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::setHandleOfElementAtWith(size_t index, const PriorityQueueHandle& handle)
{
	assert(hasElementAt(index));

	handleSetter(*(elements[index].itemPtr), handle);
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
	return hasElementAt(getLeftChildIndex(index));
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
	return hasElementAt(leftChildIndex + 1) && hasSmallerPriorityThan(elements[leftChildIndex], elements[leftChildIndex + 1]);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const Element& lhs, const Element& rhs)  const
{
	return compareFunction(keyAccessor.getKey(lhs), keyAccessor.getKey(rhs));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::validateElement(const Element& element)
{
	return element.itemPtr != nullptr;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSpaceInCurrentArray() const
{
	return insertedCount < elements.getCount();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasElementAt(size_t index) const
{
	return index < insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const
{
	return insertedCount == 0;
}
