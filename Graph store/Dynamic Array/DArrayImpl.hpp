
//-------------------------------------------------------------------------------------------------------------
//
//GET AND SET
//


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
inline bool DArray<T>::isEmpty()const
{
	return count == 0;
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


template <typename T>
void DArray<T>::setCount(int newCount)
{
	if (newCount < 0 || newCount > size)
		throw std::invalid_argument("Count must be between 0 and size");

	count = newCount;
}


template <typename T>
void DArray<T>::setSize(int newSize)
{
	if (newSize < 0)
		throw std::invalid_argument("Size cannot be negative");

	size = newSize;
}


//----------------------------------------------------------------------------------------
//
// UTILITY
//



template <typename T>
inline void DArray<T>::directInit(T* newItems, int newCount, int newSize)
{
	items = newItems;
	count = newCount;
	size = newSize;
}


template <typename T>
inline void DArray<T>::nullMembers()
{
	directInit(nullptr, 0, 0);
}


template <typename T>
inline void DArray<T>::destroyItems()
{
	delete[] items;
}



template <typename T>
inline void DArray<T>::destroyAndNullMembers()
{
	destroyItems();
	nullMembers();
}



template <typename T>
void DArray<T>::copyFrom(const DArray<T>& other)
{
	if (!other.isEmpty())
	{
		T* buffer = new T[other.size];

		for (int i = 0; i < other.count; ++i)
			buffer[i] = other.items[i];

		destroyItems();

		directInit(buffer, other.count, other.size);
	}
	else 
	{
		destroyAndNullMembers();
	}
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

	directInit(newItems, newCount, newSize);
}


template <typename T>
inline void DArray<T>::resizeIfNeeded()
{
	if (count == size)
		resize((2 * size) > 0 ? (2 * size) : 8);
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





//---------------------------------------------------------------------------------
//
//OTHER
//


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





//---------------------------------------------------------------------------------------------
//
//SPECIAL MEMBERS
//


template <typename T>
DArray<T>::DArray()
	:
	items(nullptr),
	count(0),
	size(0)
{
	;
}



template <typename T>
DArray<T>::DArray(int Size, int Count)
	:
	items(nullptr)
{
	setSize(Size);
	setCount(Count);

	if (size > 0)
		items = new T[size];
}



template <typename T>
DArray<T>::DArray(DArray<T>&& source)
	:
	items(source.items),
	count(source.count),
	size(source.size)
{
	source.nullMembers();
}



template <typename T>
DArray<T>::DArray(const DArray<T>& other)
	:
	items(nullptr)
{
	copyFrom(other);
}


//
// (!) copyFrom() frees old memory (if any)
//
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
DArray<T>& DArray<T>::operator=(DArray<T>&& source)
{
	if (this != &source)
	{
		destroyItems();
		directInit(source.items, source.count, source.size);
		source.nullMembers();
	}

	return *this;
}




template <typename T>
DArray<T>::~DArray()
{
	destroyItems();
}



//---------------------------------------------------------------------------------------------------------------------------------
//
// ADDING AND REMOVAL
//



template <typename T>
void DArray<T>::add(const T& newItem)
{
	resizeIfNeeded();

	items[count++] = newItem;
}


template <typename T>
void DArray<T>::add(T&& newItem)
{
	resizeIfNeeded();

	items[count++] = std::move(newItem);
} 



template <typename T>
void DArray<T>::addAt(int position, const T& newItem)
{
	if (position < 0 || position >= count)
		throw std::out_of_range("Index out of range");

	resizeIfNeeded();

	//empty the position
	shiftItemsOnePositionRight(position, count - 1);

	items[position] = newItem;
	++count;
}



template <typename T>
void DArray<T>::addAt(int position, T&& newItem)
{
	if (position < 0 || position >= count)
		throw std::out_of_range("Index out of range");

	resizeIfNeeded();

	//empty the position
	shiftItemsOnePositionRight(position, count - 1);

	items[position] = std::move(newItem);
	++count;
}



template <typename T>
void DArray<T>::remove(int position)
{
	if (position < 0 || position >= count)
		throw std::out_of_range("Index out of range");

	//shift items after it one pos. to the left
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
T& DArray<T>::operator[](int index)
{
	if (index < 0 || index >= count)
		throw std::out_of_range("Index out of range");

	return items[index];
}



template <typename T>
const T& DArray<T>::operator[](int index)const
{
	if (index < 0 || index >= count)
		throw std::out_of_range("Index out of range");

	return items[index];
}


template <typename T>
void DArray<T>::add(const DArray<T>& other)
{
	for (int i = 0; i < other.count; ++i)
		add(other[i]);
}


template <typename T>
void DArray<T>::add(DArray<T>&& other)
{
	for (int i = 0; i < other.count; ++i)
		add(std::move(other[i]));

	other.nullMembers();
}