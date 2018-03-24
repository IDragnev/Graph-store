

template <typename T>
DArray<T>::DArray()
{
	nullMembers();
}


template <typename T>
inline void DArray<T>::nullMembers()
{
	directlySetItemsCountAndSize(nullptr, 0, 0);
}


template <typename T>
inline void DArray<T>::directlySetItemsCountAndSize(T* newItems, int newCount, int newSize)
{
	items = newItems;
	count = newCount;
	size = newSize;
}



template <typename T>
DArray<T>::DArray(int size, int count)
	:
	items(nullptr)
{
	setSize(size);
	setCount(count);

	if (this->size > 0)
		items = new T[this->size];
}



template <typename T>
void DArray<T>::setSize(int newSize)
{
	if (newSize < 0)
		throw std::invalid_argument("Size cannot be negative");

	size = newSize;
}


template <typename T>
void DArray<T>::setCount(int newCount)
{
	if (newCount < 0 || newCount > size)
		throw std::invalid_argument("Count must be at least 0 and at most size");

	count = newCount;
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
DArray<T>::DArray(DArray<T>&& source)
{
	moveSourceInThis(source);
}



template <typename T>
inline void DArray<T>::destroyItems()
{
	delete[] items;
}



template <typename T>
void DArray<T>::moveSourceInThis(DArray<T>& source)
{
	directlySetItemsCountAndSize(source.items, source.count, source.size);
	source.nullMembers();
}



template <typename T>
DArray<T>::DArray(const DArray<T>& other)
	:
	items(nullptr)
{
	copyFrom(other);
}



template <typename T>
DArray<T>& DArray<T>::operator=(const DArray<T>& other)
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

		for (int i = 0; i < other.count; ++i)
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
DArray<T>::~DArray()
{
	destroyItems();
}



template <typename T>
void DArray<T>::insert(const T& newItem)
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
void DArray<T>::resize(int newSize)
{
	T* newItems = new T[newSize];

	int newCount = (count <= newSize) ? count : newSize;

	for (int i = 0; i < newCount; ++i)
		newItems[i] = items[i];

	destroyItems();

	directlySetItemsCountAndSize(newItems, newCount, newSize);
}
 



template <typename T>
void DArray<T>::insertAt(int position, const T& newItem)
{
	throwExceptionIfInvalidIndex(position);

	enlargeIfFull();

	shiftItemsOnePositionRight(position, count - 1);

	items[position] = newItem;
	++count;
}



template <typename T>
void DArray<T>::throwExceptionIfInvalidIndex(int index)const
{
	if (index < 0 || index >= count)
		throw std::out_of_range("Index out of range");
}



//
// (!) the function assumes that count < size, so that shifting the last 
//     item to the right will not write outside the array
//
template <typename T>
inline void DArray<T>::shiftItemsOnePositionRight(int start, int end)
{
	assert(end < count && count < size);

	while (end >= start)
	{
		items[end + 1] = items[end];
		--end;
	}
}



template <typename T>
inline void DArray<T>::shiftItemsOnePositionLeft(int start, int end)
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
inline void DArray<T>::ensureSize(int newSize)
{
	if (newSize > size)
		resize(newSize);
}



template <typename T>
inline void DArray<T>::shrink(int newSize)
{
	if (newSize < 0 || newSize > size)
		throw std::invalid_argument("Invalid size sent");

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
void DArray<T>::remove(int position)
{
	throwExceptionIfInvalidIndex(position);

	shiftItemsOnePositionLeft(position + 1, count - 1);

	--count;
}



template <typename T>
int DArray<T>::search(const T& searchedItem)const
{
	for (int i = 0; i < count; ++i)
	{
		if (items[i] == searchedItem)
			return i;
	}

	return -1;
}



template <typename T>
T& DArray<T>::operator[](int position)
{
	throwExceptionIfInvalidIndex(position);

	return items[position];
}



template <typename T>
const T& DArray<T>::operator[](int position)const
{
	throwExceptionIfInvalidIndex(position);

	return items[position];
}



template <typename T>
void DArray<T>::insert(const DArray<T>& other)
{
	for (int i = 0; i < other.count; ++i)
		insert(other[i]);
}



template <typename T>
inline int DArray<T>::getCount()const
{
	return count;
}


template <typename T>
inline int DArray<T>::getSize()const
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
