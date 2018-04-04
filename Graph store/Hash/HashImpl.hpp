/*
     MIN_TABLE_SIZE is 3 because it should be small enough to be rarely used in the constructor,
	 big enough to have empty slots for smaller counts ( when the table holds 1 or 2 items ),
	 and small enough to be able to halve small tables after removal ( when size is 6,7,8 )
*/


template <typename Item, typename Key, typename KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(size_t expectedCount)
	:
	tableSize( calculateAppropriateSize(expectedCount) ),
	insertedCount(0),
	table(tableSize, tableSize)
{
	nullTable();
}


//
// ( 3 * expectedSize ) / 2 is used because if all the expected items
// are inserted, the load factor will be 2/3 
// 
template <typename Item, typename Key, typename KeyAccessor>
size_t Hash<Item, Key, KeyAccessor>::calculateAppropriateSize(size_t expectedCount)
{
	if (expectedCount == 0)
		throw std::invalid_argument("Expected size must be positive!");

	return (expectedCount < MIN_TABLE_SIZE) ? MIN_TABLE_SIZE : (3 * expectedCount) / 2;
}


template <typename Item, typename Key, typename KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(Hash<Item, Key, KeyAccessor>&& source)
	:
	tableSize(MIN_TABLE_SIZE),
	insertedCount(0),
	table(MIN_TABLE_SIZE, MIN_TABLE_SIZE), 
	hashFunction(std::move(source.hashFunction)),
	keyAccessor(std::move(source.keyAccessor))
{
	this->nullTable();

	std::swap(this->table, source.table);
	std::swap(this->tableSize, source.tableSize);
	std::swap(this->insertedCount, source.insertedCount);
}


template <typename Item, typename Key, typename KeyAccessor>
inline void Hash<Item, Key, KeyAccessor>::nullTable()
{
	for (size_t i = 0; i < tableSize; ++i)
		table[i] = nullptr;
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
	std::swap(this->tableSize, temporary.tableSize);
	std::swap(this->insertedCount, temporary.insertedCount);
	std::swap(this->table, temporary.table);
	std::swap(this->hashFunction, temporary.hashFunction);
	std::swap(this->keyAccessor, temporary.keyAccessor);
}


template <typename Item, typename Key, typename KeyAccessor>
inline Item* Hash<Item, Key, KeyAccessor>::search(const Key& key)
{
	const long INDEX = getIndexByKey(key);

	return isValidPosition(INDEX) ? table[INDEX] : nullptr;
}


//
// the function is not const because KeyAccessor and hashFunction
// could have non-const operator()
//
template <typename Item, typename Key, typename KeyAccessor>
long Hash<Item, Key, KeyAccessor>::getIndexByKey(const Key& key)
{
	if ( ! isEmpty() )
	{
		size_t index = hashFunction(key) % tableSize;

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
inline bool Hash<Item, Key, KeyAccessor>::isEmpty()const
{
	return insertedCount == 0;
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isValidPosition(long index)
{
	return index >= 0;
}


template <typename Item, typename Key, typename KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	const long INDEX = getIndexByKey(key);

	if ( isValidPosition(INDEX) )
	{
		Item* result = extractItemFromTable(INDEX);

		if ( shouldHalveTable() )
			resize(tableSize / 2);
		else
			rehashCluster( (INDEX + 1) % tableSize );
		
		return result;
	}

	return nullptr;
}


template <typename Item, typename Key, typename KeyAccessor>
inline Item* Hash<Item, Key, KeyAccessor>::extractItemFromTable(size_t index)
{
	Item* result = table[index];

	table[index] = nullptr;
	--insertedCount;

	return result;
}



template <typename Item,typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::shouldHalveTable()const
{
	return (6 * insertedCount <= tableSize) && (tableSize / 2 >= MIN_TABLE_SIZE);
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(size_t newSize)
{
	//must have at least one empty pos. after resize
	assert(newSize >= MIN_TABLE_SIZE && newSize > insertedCount);
	
	const size_t OLD_TABLE_SIZE = tableSize;

	DArray<Item*> temp(newSize, newSize);

	std::swap(table, temp);
	
	tableSize = newSize;
	insertedCount = 0;
	nullTable();

	for (size_t i = 0; i < OLD_TABLE_SIZE; ++i)
	{
		if (temp[i] != nullptr)
			insert( *temp[i] );
	}
}




template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::rehashCluster(size_t start)
{
	size_t positionToEmpty = start;

	while ( table[positionToEmpty] != nullptr )
	{
		insert( *extractItemFromTable(positionToEmpty) );

		positionToEmpty = (positionToEmpty + 1) % tableSize;
	}
}



template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::insert(Item& item)
{
	if ( shouldDoubleTable() )
		resize(tableSize * 2);

	size_t index = hashFunction( keyAccessor(item) ) % tableSize;

	while(table[index] != nullptr)
		index = (index + 1) % tableSize;

	table[index] = &item;
	++insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::shouldDoubleTable()const
{
	return (3 * insertedCount >= 2 * tableSize);
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::empty()
{
	table.shrink(MIN_TABLE_SIZE);

	tableSize = MIN_TABLE_SIZE;
	insertedCount = 0;

	nullTable();
}


template <typename Item, typename Key, typename KeyAccessor>
inline size_t Hash<Item, Key, KeyAccessor>::getCount()const
{
	return insertedCount;
}

