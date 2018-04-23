/*
     MIN_TABLE_SIZE is 3 because it should be small enough to be rarely used in the constructor,
	 big enough to have empty slots for smaller counts ( when the table holds 1 or 2 items ),
	 and small enough to be able to halve small tables after removal ( when size is 6,7,8 )
*/


template <typename Item, typename Key, typename KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(sizeType expectedCount) :
	tableSize( calculateAppropriateSize(expectedCount) ),
	insertedCount(0),
	table(tableSize, tableSize)
{
	nullifyTable();
}


template <typename Item, typename Key, typename KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(Hash<Item, Key, KeyAccessor>&& source) :
	tableSize(MIN_TABLE_SIZE),
	insertedCount(0),
	table(MIN_TABLE_SIZE, MIN_TABLE_SIZE), 
	hashFunction(std::move(source.hashFunction)),
	keyAccessor(std::move(source.keyAccessor))
{
	nullifyTable();

	std::swap(table, source.table);
	std::swap(tableSize, source.tableSize);
	std::swap(insertedCount, source.insertedCount);
}


template <typename Item, typename Key, typename KeyAccessor>
inline Hash<Item, Key, KeyAccessor>& Hash<Item, Key, KeyAccessor>::operator=(Hash<Item, Key, KeyAccessor>&& other)
{
	if (this != &other)
	{
		swapContentsWithReconstructedParameter(std::move(other));
	}

	return *this;
}


template <typename Item, typename Key, typename KeyAccessor>
inline Hash<Item, Key, KeyAccessor>& Hash<Item, Key, KeyAccessor>::operator=(const Hash<Item, Key, KeyAccessor>& other)
{
	if (this != &other)
	{
		swapContentsWithReconstructedParameter(other);
	}

	return *this;
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::swapContentsWithReconstructedParameter(Hash<Item, Key, KeyAccessor> temporary)
{
	std::swap(tableSize, temporary.tableSize);
	std::swap(insertedCount, temporary.insertedCount);
	std::swap(table, temporary.table);
	std::swap(hashFunction, temporary.hashFunction);
	std::swap(keyAccessor, temporary.keyAccessor);
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::insert(Item& item)
{
	if (isFillingUp())
		resize(tableSize * 2);

	sizeType index = hashFunction(keyAccessor(item)) % tableSize;

	while(table[index] != nullptr)
		index = (index + 1) % tableSize;

	table[index] = &item;
	++insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor>
inline Item* Hash<Item, Key, KeyAccessor>::search(const Key& key)
{
	const long index = searchTableAndGetIndex(key);

	return isValidSlotPosition(index) ? table[index] : nullptr;
}


template <typename Item, typename Key, typename KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	const long index = searchTableAndGetIndex(key);

	if (isValidSlotPosition(index))
	{
		Item* result = extractItemFromTableAt(index);

		if (isTooEmpty() && canBeHalved())
			resize(tableSize / 2);
		else
			rehashCluster((index + 1) % tableSize);
		
		return result;
	}

	return nullptr;
}


//
// the function is not const because KeyAccessor and hashFunction
// could have non-const operator()
//
template <typename Item, typename Key, typename KeyAccessor>
long Hash<Item, Key, KeyAccessor>::searchTableAndGetIndex(const Key& key)
{
	if (!isEmpty())
	{
		sizeType index = hashFunction(key) % tableSize;

		while (table[index] != nullptr)
		{
			if (keyAccessor( *(table[index]) ) == key)
				return index;

			index = (index + 1) % tableSize;
		}
	}

	return -1;
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isValidSlotPosition(long index)
{
	return index >= 0;
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(sizeType newSize)
{
	//must have at least one empty position after resize
	assert(newSize >= MIN_TABLE_SIZE && newSize > insertedCount);
	
	const sizeType oldTableSize = tableSize;

	DArray<Item*> temp(newSize, newSize);

	std::swap(table, temp);
	
	tableSize = newSize;
	insertedCount = 0;
	nullifyTable();

	for (sizeType i = 0; i < oldTableSize; ++i)
	{
		if (temp[i] != nullptr)
			insert( *temp[i] );
	}
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::rehashCluster(sizeType start)
{
	sizeType positionToEmpty = start;

	while (table[positionToEmpty] != nullptr)
	{
		insert(*extractItemFromTableAt(positionToEmpty));

		positionToEmpty = (positionToEmpty + 1) % tableSize;
	}
}


template <typename Item, typename Key, typename KeyAccessor>
inline Item* Hash<Item, Key, KeyAccessor>::extractItemFromTableAt(sizeType index)
{
	Item* result = table[index];

	table[index] = nullptr;
	--insertedCount;

	return result;
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::empty()
{
	table.shrink(MIN_TABLE_SIZE);

	tableSize = MIN_TABLE_SIZE;
	insertedCount = 0;

	nullifyTable();
}


//
// ( 3 * expectedSize ) / 2 is used because if all the expected items
// are inserted, the load factor will be 2/3 
// 
template <typename Item, typename Key, typename KeyAccessor>
typename Hash<Item,Key,KeyAccessor>::sizeType Hash<Item, Key, KeyAccessor>::calculateAppropriateSize(sizeType expectedCount)
{
	if (expectedCount == 0)
		throw std::invalid_argument("Expected size must be positive!");

	return (expectedCount < MIN_TABLE_SIZE) ? MIN_TABLE_SIZE : (3 * expectedCount) / 2;
}


template <typename Item, typename Key, typename KeyAccessor>
inline void Hash<Item, Key, KeyAccessor>::nullifyTable()
{
	for (sizeType i = 0; i < tableSize; ++i)
		table[i] = nullptr;
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isEmpty() const
{
	return insertedCount == 0;
}


template <typename Item,typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isTooEmpty() const
{
	return (6 * insertedCount <= tableSize);
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::canBeHalved() const
{
	return (tableSize / 2 >= MIN_TABLE_SIZE);
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isFillingUp() const
{
	return 3 * insertedCount >= 2 * tableSize;
}


template <typename Item, typename Key, typename KeyAccessor>
inline typename Hash<Item,Key,KeyAccessor>::sizeType Hash<Item, Key, KeyAccessor>::getCount() const
{
	return insertedCount;
}

