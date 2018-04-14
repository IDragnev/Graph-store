

template <typename T>
DArray<T>::DArray() :
	size(0),
	count(0),
	items(nullptr)
{
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
DArray<T>::DArray(DArray<T>&& source) :
	count(source.count),
	size(source.size),
	items(source.items)
{
	source.nullifyMembers();
}


template <typename T>
DArray<T>::DArray(const DArray<T>& source) :
	DArray<T>()
{
	copyFrom(source);
}


template <typename T>
DArray<T>& DArray<T>::operator=(const DArray<T>& rhs)
{
	if (this != &rhs)
	{
		swapContentsWithReconstructedParameter(rhs);
	}

	return *this;
}


template <typename T>
DArray<T>& DArray<T>::operator=(DArray<T>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWithReconstructedParameter(std::move(rhs));
	}

	return *this;
}


template <typename T>
inline DArray<T>::~DArray()
{
	destroyItems();
}


template <typename T>
void DArray<T>::copyFrom(const DArray<T>& source)
{
	DArray<T> temporary(source.size, source.count);

	for (sizeType i = 0; i < source.count; ++i)
		temporary.items[i] = source.items[i];

	swapContentsWithReconstructedParameter(std::move(temporary));
}


template <typename T>
void DArray<T>::resize(sizeType newSize)
{
	sizeType newCount = (count <= newSize) ? count : newSize;

	DArray<T> temporary(newSize, newCount);

	for (sizeType i = 0; i < newCount; ++i)
		temporary.items[i] = items[i];

	swapContentsWithReconstructedParameter(std::move(temporary));
}


template <typename T>
inline void DArray<T>::swapContentsWithReconstructedParameter(DArray<T> temporary)
{
	std::swap(size, temporary.size);
	std::swap(count, temporary.count);
	std::swap(items, temporary.items);
}


template <typename T>
inline void DArray<T>::empty()
{
	destroyItems();
	nullifyMembers();
}


template <typename T>
void DArray<T>::shrink(sizeType newSize)
{
	if (newSize > size)
		throw std::invalid_argument("Cannot shrink to bigger size");

	if (newSize == 0)
	{
		destroyItems();
		nullifyMembers();
	}
	else if (newSize != size)
	{
		resize(newSize);
	}
}


template <typename T>
inline void DArray<T>::ensureSize(sizeType newSize)
{
	if (newSize > size)
		resize(newSize);
}


template <typename T>
inline void DArray<T>::insert(const T& newItem)
{
	enlargeIfFull();

	items[count++] = newItem;
}


template <typename T>
void DArray<T>::insertAt(sizeType position, const T& newItem)
{
	if (position != count)
	{
		throwExceptionIfInvalidIndex(position);

		enlargeIfFull();

		shiftItemsOnePositionRight(position, count - 1);

		items[position] = newItem;
		++count;
	}
	else
	{
		insert(newItem);
	}
}


template <typename T>
inline void DArray<T>::insert(const DArray<T>& other)
{
	for (sizeType i = 0; i < other.count; ++i)
		insert(other[i]);
}


template <typename T>
inline void DArray<T>::enlargeIfFull()
{
	assert(count <= size);

	if (count == size)
		resize(size > 0 ? (2 * size) : 8);
}


template <typename T>
inline void DArray<T>::remove(sizeType position)
{
	throwExceptionIfInvalidIndex(position);

	shiftItemsOnePositionLeft(position + 1, --count);
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
inline void DArray<T>::destroyItems()
{
	delete[] items;
}


template <typename T>
inline void DArray<T>::nullifyMembers()
{
	size = 0;
	count = 0;
	items = nullptr;
}


template <typename T>
inline bool DArray<T>::isEmpty()const
{
	return count == 0;
}


template <typename T>
inline void DArray<T>::throwExceptionIfInvalidIndex(sizeType index)const
{
	if (index >= count)
		throw std::out_of_range("Index out of range");
}


template <typename T>
inline void DArray<T>::shiftItemsOnePositionRight(sizeType start, sizeType end)
{
	assert(end + 1 < size);

	for (sizeType i = end + 1; i > start; --i)
		items[i] = items[i - 1];
}


template <typename T>
inline void DArray<T>::shiftItemsOnePositionLeft(sizeType start, sizeType end)
{
	assert(start > 0 && end < size);

	for (sizeType i = start - 1; i < end; ++i)
		items[i] = items[i + 1];
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

