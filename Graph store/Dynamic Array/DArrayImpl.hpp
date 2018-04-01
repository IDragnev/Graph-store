

template <typename T>
inline DArray<T>::DArray()
{
	nullMembers();
}


template <typename T>
inline void DArray<T>::nullMembers()
{
	directlySetItemsCountAndSize(nullptr, 0, 0);
}


template <typename T>
inline void DArray<T>::directlySetItemsCountAndSize(T* newItems, sizeType newCount, sizeType newSize)
{
	items = newItems;
	count = newCount;
	size = newSize;
}



template <typename T>
DArray<T>::DArray(sizeType size, sizeType count) :
	size(size),
	items(nullptr)
{
	setCount(count);

	if (this->size > 0)
		items = new T[this->size];
}



template <typename T>
inline void DArray<T>::setCount(sizeType newCount)
{
	if (newCount <= size)
	{
		count = newCount;
	}
	else
		throw std::invalid_argument("Count cannot exceed size");
}



template <typename T>
inline DArray<T>& DArray<T>::operator=(DArray<T>&& source)
{
	if (this != &source)
	{
		destroyItems();
		moveParameterInThis(source);
	}

	return *this;
}



template <typename T>
inline DArray<T>::DArray(DArray<T>&& source)
{
	moveParameterInThis(source);
}



template <typename T>
inline void DArray<T>::destroyItems()
{
	delete[] items;
}



template <typename T>
inline void DArray<T>::moveParameterInThis(DArray<T>& source)
{
	directlySetItemsCountAndSize(source.items, source.count, source.size);
	source.nullMembers();
}



template <typename T>
inline DArray<T>::DArray(const DArray<T>& other)
{
	nullMembers();
	copyFrom(other);
}



template <typename T>
inline DArray<T>& DArray<T>::operator=(const DArray<T>& other)
{
	if (this != &other)
	{
		copyFrom(other);
	}

	return *this;
}



template <typename T>
void DArray<T>::copyFrom(const DArray<T>& other)
{
	if ( ! other.isEmpty() )
	{
		DArray<T> temporary(other.size, other.count);

		for (sizeType i = 0; i < other.count; ++i)
			temporary.items[i] = other.items[i];

		std::swap(*this, temporary);
	}
	else 
	{
		destroyAndNullMembers();
	}
}



template <typename T>
inline bool DArray<T>::isEmpty()const
{
	return count == 0;
}


template <typename T>
inline void DArray<T>::destroyAndNullMembers()
{
	destroyItems();
	nullMembers();
}   



template <typename T>
inline DArray<T>::~DArray()
{
	destroyItems();
}



template <typename T>
inline void DArray<T>::insert(const T& newItem)
{
	enlargeIfFull();

	items[count++] = newItem;
}


template <typename T>
inline void DArray<T>::enlargeIfFull()
{
	if (count == size)
		resize((2 * size) > 0 ? (2 * size) : 8);
}



template <typename T>
void DArray<T>::resize(sizeType newSize)
{
	sizeType newCount = (count <= newSize) ? count : newSize;

	DArray<T> temporary(newSize, newCount);

	for (sizeType i = 0; i < newCount; ++i)
		temporary.items[i] = items[i];

	std::swap(*this, temporary);
}
 


template <typename T>
void DArray<T>::insertAt(sizeType position, const T& newItem)
{
	if (position == count)
		insert(newItem);
	else
	{
		throwExceptionIfInvalidIndex(position);

		enlargeIfFull();

		shiftItemsOnePositionRight(position, count - 1);

		items[position] = newItem;
		++count;
	}
}



template <typename T>
inline void DArray<T>::throwExceptionIfInvalidIndex(sizeType index)const
{
	if (index >= count)
		throw std::out_of_range("Index out of range");
}



//
//  the function assumes that count < size, so that shifting the last 
//  item to the right will not write outside the array
//
template <typename T>
inline void DArray<T>::shiftItemsOnePositionRight(sizeType start, sizeType end)
{
	assert(end < count && count < size);

	for (sizeType i = end + 1; i > start; --i)
		items[i] = items[i - 1];
}



template <typename T>
inline void DArray<T>::shiftItemsOnePositionLeft(sizeType start, sizeType end)
{
	assert(start > 0);

	for (sizeType i = start - 1; i < end; ++i)
		items[i] = items[i + 1];
}



template <typename T>
inline void DArray<T>::empty()
{
	destroyAndNullMembers();
}



template <typename T>
inline void DArray<T>::ensureSize(sizeType newSize)
{
	if (newSize > size)
		resize(newSize);
}



template <typename T>
inline void DArray<T>::shrink(sizeType newSize)
{
	if (newSize > size)
		throw std::invalid_argument("Cannot shrink to bigger size");

	if (newSize == 0)
	{
		destroyAndNullMembers();
	}
	else if (newSize != size)
	{
		resize(newSize);
	}
}



template <typename T>
inline void DArray<T>::remove(sizeType position)
{
	throwExceptionIfInvalidIndex(position);

	shiftItemsOnePositionLeft(position + 1, count - 1);

	--count;
}



template <typename T>
inline T& DArray<T>::operator[](sizeType position)
{
	throwExceptionIfInvalidIndex(position);

	return items[position];
}



template <typename T>
inline const T& DArray<T>::operator[](sizeType position)const
{
	throwExceptionIfInvalidIndex(position);

	return items[position];
}



template <typename T>
inline void DArray<T>::insert(const DArray<T>& other)
{
	for (sizeType i = 0; i < other.count; ++i)
		insert(other[i]);
}



template <typename T>
inline typename DArray<T>::sizeType DArray<T>::getCount()const
{
	return count;
}


template <typename T>
inline typename DArray<T>::sizeType DArray<T>::getSize()const
{
	return size;
}


template <typename T>
inline T* DArray<T>::getItems()
{
	return items;
}


template <typename T>
inline const T* DArray<T>::getItems()const
{
	return items;
}
