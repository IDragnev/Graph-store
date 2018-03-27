

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
inline void DArray<T>::directlySetItemsCountAndSize(T* newItems, size_type newCount, size_type newSize)
{
	items = newItems;
	count = newCount;
	size = newSize;
}



template <typename T>
DArray<T>::DArray(size_type size, size_type count)
	:
	items(nullptr)
{
	setSize(size);
	setCount(count);

	if (this->size > 0)
		items = new T[this->size];
}



template <typename T>
inline void DArray<T>::setSize(size_type newSize)
{
	size = newSize;
}



template <typename T>
void DArray<T>::setCount(size_type newCount)
{
	if (newCount <= size)
	{
		count = newCount;
	}
	else
		throw std::invalid_argument("Count cannot exceed size");
}



template <typename T>
DArray<T>& DArray<T>::operator=(DArray<T>&& source)
{
	if (this != &source)
	{
		destroyItems();
		moveSourceInThis(source);
	}

	return *this;
}



template <typename T>
inline DArray<T>::DArray(DArray<T>&& source)
{
	moveSourceInThis(source);
}



template <typename T>
inline void DArray<T>::destroyItems()
{
	delete[] items;
}



template <typename T>
inline void DArray<T>::moveSourceInThis(DArray<T>& source)
{
	directlySetItemsCountAndSize(source.items, source.count, source.size);
	source.nullMembers();
}



template <typename T>
inline DArray<T>::DArray(const DArray<T>& other)
	:
	items(nullptr)
{
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
		T* buffer = new T[other.size];

		for (size_type i = 0; i < other.count; ++i)
			buffer[i] = other.items[i];

		destroyItems();

		directlySetItemsCountAndSize(buffer, other.count, other.size);
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



//
// (!) if items == nullptr, then count will be 0 
//   and the null pointer will not be touched
//
template <typename T>
void DArray<T>::resize(size_type newSize)
{
	T* newItems = new T[newSize];

	size_type newCount = (count <= newSize) ? count : newSize;

	for (size_type i = 0; i < newCount; ++i)
		newItems[i] = items[i];

	destroyItems();

	directlySetItemsCountAndSize(newItems, newCount, newSize);
}
 



template <typename T>
void DArray<T>::insertAt(size_type position, const T& newItem)
{
	throwExceptionIfInvalidIndex(position);

	enlargeIfFull();

	shiftItemsOnePositionRight(position, count - 1);

	items[position] = newItem;
	++count;
}



template <typename T>
inline void DArray<T>::throwExceptionIfInvalidIndex(size_type index)const
{
	if (index >= count)
		throw std::out_of_range("Index out of range");
}



//
// (!) the function assumes that count < size, so that shifting the last 
//     item to the right will not write outside the array
//
template <typename T>
inline void DArray<T>::shiftItemsOnePositionRight(size_type start, size_type end)
{
	assert(end < count && count < size);

	while (end >= start)
	{
		items[end + 1] = items[end];
		--end;
	}
}



template <typename T>
inline void DArray<T>::shiftItemsOnePositionLeft(size_type start, size_type end)
{
	assert(start > 0);

	while (start <= end)
	{
		items[start - 1] = items[start];
		++start;
	}
}



template <typename T>
inline void DArray<T>::empty()
{
	destroyAndNullMembers();
}



template <typename T>
inline void DArray<T>::ensureSize(size_type newSize)
{
	if (newSize > size)
		resize(newSize);
}



template <typename T>
inline void DArray<T>::shrink(size_type newSize)
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
inline void DArray<T>::remove(size_type position)
{
	throwExceptionIfInvalidIndex(position);

	shiftItemsOnePositionLeft(position + 1, count - 1);

	--count;
}



template <typename T>
inline T& DArray<T>::operator[](size_type position)
{
	throwExceptionIfInvalidIndex(position);

	return items[position];
}



template <typename T>
inline const T& DArray<T>::operator[](size_type position)const
{
	throwExceptionIfInvalidIndex(position);

	return items[position];
}



template <typename T>
inline void DArray<T>::insert(const DArray<T>& other)
{
	for (size_type i = 0; i < other.count; ++i)
		insert(other[i]);
}



template <typename T>
inline typename DArray<T>::size_type DArray<T>::getCount()const
{
	return count;
}


template <typename T>
inline typename DArray<T>::size_type DArray<T>::getSize()const
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
