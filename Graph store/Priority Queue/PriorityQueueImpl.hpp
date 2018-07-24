#include <assert.h>


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, CompareFunction, HandleSetter>::PriorityQueue() :
	elements(),
	insertedCount(0)
{
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::isEmpty() const
{
	return insertedCount == 0;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, CompareFunction, HandleSetter>::insert(Element&& newElement)
{
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


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::hasSpaceInCurrentArray() const
{
	return elements.getCount() > insertedCount;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, CompareFunction, HandleSetter>::buildHeapOfCurrentElements()
{
	assert(!isEmpty());

	size_t elementToSiftIndex = getLastNonLeafIndex();

	while (elementToSiftIndex >= 0)
	{
		siftDown(elementToSiftIndex--);
	}
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
size_t PriorityQueue<Item, Key, CompareFunction, HandleSetter>::getLastNonLeafIndex() const
{
	assert(!isEmpty());

	return (insertedCount > 1) ? (insertedCount - 2) / 2 : 0;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, CompareFunction, HandleSetter>::siftDown(size_t index)
{
	assert(isValid(index));
	Element hand = std::move(elements[index]);

	while (hasChildren(index))
	{
		size_t optimalChildIndex = getOptimalChildIndex(index);

		if (compare(hand, elements[optimalChildIndex]))  // MAX <
		{
			elements[index] = elements[optimalChildIndex];
			updateHandleOfElementAt(index);

			index = optimalChildIndex;
		}
		else 
		{
			break;
		}
	}

	elements[index] = std::move(hand);
	updateHandleOfElementAt(index);
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, CompareFunction, HandleSetter>::siftUp(size_t index)
{
	assert(isValid(index));
	Element hand = std::move(elements[index]);

	while (hasParent(index))
	{
		size_t parentIndex = getParentIndex(index);

		if (compare(elements[parentIndex], hand))     // MAX < 
		{
			elements[index] = elements[parentIndex];
			updateHandleOfElementAt(index);

			index = parentIndex;
		}
		else
		{
			break;
		}
	}

	elements[index] = std::move(hand);
	updateHandleOfElementAt(index);
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::hasChildren(size_t index) const
{
	return isValid(getLeftChildIndex(index));
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
size_t PriorityQueue<Item, Key, CompareFunction, HandleSetter>::getLeftChildIndex(size_t index) const
{
	assert(isValid(index));

	return 2 * index + 1;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::isValid(size_t index) const
{
	return index < insertedCount;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
size_t PriorityQueue<Item, Key, CompareFunction, HandleSetter>::getParentIndex(size_t index) const
{
	assert(hasParent(index));

	return (index - 1) / 2;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::hasParent(size_t index) const
{
	assert(isValid(index));

	return index > 0;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
size_t PriorityQueue<Item, Key, CompareFunction, HandleSetter>::getOptimalChildIndex(size_t index) const
{
	assert(hasChildren(index));

	size_t result = getLeftChildIndex(index);

	if (hasOptimalRightSibling(result))
	{
		++result;
	}

	return result;
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::hasOptimalRightSibling(size_t leftChildIndex) const
{
	return isValid(leftChildIndex + 1) && compare(elements[leftChildIndex], elements[leftChildIndex + 1]);
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::compare(const Element& lhs, const Element& rhs)  const
{
	return compareFunction(lhs.key, rhs.key);
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, CompareFunction, HandleSetter>::updateHandleOfElementAt(size_t index)
{
	assert(isValid(index));

	handleSetter(elements[index].itemPtr, index);
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
const Item& PriorityQueue<Item, Key, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());

	return *(elements[0].itemPtr);
}

