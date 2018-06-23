

template <typename T>
DArray<T>::DArray() :
	size(0),
	count(0),
	items(nullptr)
{ 
}


template <typename T>
DArray<T>::DArray(unsignedInteger size, unsignedInteger count) :
	size(size),
	items(nullptr)
{
	setCount(count);

	if (this->size > 0)
	{
		items = new T[this->size];
	}
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

	for (unsignedInteger i = 0; i < source.count; ++i)
	{
		temporary.items[i] = source.items[i];
	}

	swapContentsWithReconstructedParameter(std::move(temporary));
}


template <typename T>
void DArray<T>::resize(unsignedInteger newSize)
{
	unsignedInteger newCount = (count <= newSize) ? count : newSize;

	DArray<T> temporary(newSize, newCount);

	for (unsignedInteger i = 0; i < newCount; ++i)
	{
		temporary.items[i] = items[i];
	}

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
void DArray<T>::shrink(unsignedInteger newSize)
{
	assert(newSize <= size);

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
inline void DArray<T>::ensureSize(unsignedInteger newSize)
{
	if (newSize > size)
	{
		resize(newSize);
	}
}


template <typename T>
inline void DArray<T>::insert(const T& newItem)
{
	enlargeIfFull();

	items[count++] = newItem;
}


template <typename T>
void DArray<T>::insertAt(unsignedInteger position, const T& newItem)
{
	if (position != count)
	{
		assert(hasItemAt(position));

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
	for (unsignedInteger i = 0; i < other.count; ++i)
	{
		insert(other[i]);
	}
}


template <typename T>
inline void DArray<T>::enlargeIfFull()
{
	assert(count <= size);

	if (count == size)
	{
		resize(size > 0 ? (GROWTH_FACTOR * size) : DEFAULT_SIZE);
	}
}


template <typename T>
inline void DArray<T>::removeAt(unsignedInteger position)
{
	assert(hasItemAt(position));

	shiftItemsOnePositionLeft(position + 1, --count);
}


template <typename T>
inline T& DArray<T>::operator[](unsignedInteger position)
{
	return const_cast<T&>( static_cast<const DArray<T>&>(*this)[position] );
}


template <typename T>
inline const T& DArray<T>::operator[](unsignedInteger position) const
{
	assert(hasItemAt(position));

	return items[position];
}


template <typename T>
inline void DArray<T>::setCount(unsignedInteger newCount)
{
	assert(newCount <= size);

	count = newCount;
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
inline bool DArray<T>::isEmpty() const
{
	return count == 0;
}


template <typename T>
inline void DArray<T>::shiftItemsOnePositionRight(unsignedInteger start, unsignedInteger end)
{
	assert(end + 1 < size);

	for (unsignedInteger i = end + 1; i > start; --i)
	{
		items[i] = items[i - 1];
	}
}


template <typename T>
inline void DArray<T>::shiftItemsOnePositionLeft(unsignedInteger start, unsignedInteger end)
{
	assert(start > 0 && end < size);

	for (unsignedInteger i = start - 1; i < end; ++i)
	{
		items[i] = items[i + 1];
	}
}


template <typename T>
inline bool DArray<T>::hasItemAt(unsignedInteger position) const
{
	return position < count;
}


template <typename T>
inline typename DArray<T>::unsignedInteger DArray<T>::getCount() const
{
	return count;
}


template <typename T>
inline typename DArray<T>::unsignedInteger DArray<T>::getSize() const
{
	return size;
}


template <typename T>
inline typename DArray<T>::Iterator DArray<T>::getHeadIterator()
{
	return Iterator(0, this);
}


template <typename T>
inline typename DArray<T>::ConstIterator DArray<T>::getHeadConstIterator() const
{
	return ConstIterator(0, this);
}