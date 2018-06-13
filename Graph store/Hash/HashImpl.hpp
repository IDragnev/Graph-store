/*
     MIN_TABLE_SIZE is 3 because it should be small enough to be rarely used in the constructor,
	 big enough to have empty slots for smaller counts ( when the table holds 1 or 2 items ),
	 and small enough to be able to halve small tables after removal ( when size is 6,7,8 )
*/


template <typename Item, typename Key, typename KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(unsignedInteger expectedCount) 
{
	toEmptyStateOfSize(calculateAppropriateSize(expectedCount));
}


template <typename Item, typename Key, typename KeyAccessor>
Hash<Item, Key, KeyAccessor>::Hash(Hash<Item, Key, KeyAccessor>&& source)
{
	toEmptyStateOfSize(MIN_TABLE_SIZE);

	std::swap(table, source.table);
	std::swap(tableSize, source.tableSize);
	std::swap(insertedCount, source.insertedCount);

	hashFunction = std::move(source.hashFunction);
	keyAccessor = std::move(source.keyAccessor);
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
	{
		resize(tableSize * GROWTH_FACTOR);
	}

	unsignedInteger index = hashFunction(keyAccessor(item)) % tableSize;

	while (table[index])
	{
		index = (index + 1) % tableSize;
	}

	table[index] = &item;
	++insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor>
inline const Item* Hash<Item, Key, KeyAccessor>::search(const Key& key) const
{
	const long index = searchTableAndGetIndex(key);

	return index >= 0 ? table[index] : nullptr;
}


template <typename Item, typename Key, typename KeyAccessor>
inline Item* Hash<Item, Key, KeyAccessor>::search(const Key& key)
{
	return const_cast<Item*>( static_cast<const Hash<Item, Key, KeyAccessor>&>(*this).search(key) );
}


template <typename Item, typename Key, typename KeyAccessor>
Item* Hash<Item, Key, KeyAccessor>::remove(const Key& key)
{
	const long index = searchTableAndGetIndex(key);

	if (index >= 0)
	{
		Item* result = extractItemFromTableAt(index);

		if (hasTooManyEmptySlots() && canBeShrinked())
		{
			resize(tableSize / GROWTH_FACTOR);
		}
		else
		{
			rehashCluster((index + 1) % tableSize);
		}
		
		return result;
	}

	return nullptr;
}


template <typename Item, typename Key, typename KeyAccessor>
long Hash<Item, Key, KeyAccessor>::searchTableAndGetIndex(const Key& key) const
{
	if (!isEmpty())
	{
		unsignedInteger index = hashFunction(key) % tableSize;

		while (table[index])
		{
			if (keyAccessor( *(table[index]) ) == key)
			{
				return index;
			}

			index = (index + 1) % tableSize;
		}
	}

	return -1;
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::resize(unsignedInteger newSize)
{
	//must have at least one empty position after resize
	assert(newSize >= MIN_TABLE_SIZE && newSize > insertedCount);
	
	DArray<Item*> oldTable(std::move(table));
	
	try
	{
		toEmptyStateOfSize(newSize);
		insertAllItemsFrom(oldTable);
	}
	catch (std::bad_alloc&)
	{
		table = std::move(oldTable);
		throw;
	}
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::toEmptyStateOfSize(unsignedInteger size)
{
	assert(size >= MIN_TABLE_SIZE);

	table = DArray<Item*>(size, size);
	tableSize = size;
	insertedCount = 0;
	nullify(table);
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::insertAllItemsFrom(DArray<Item*>& table)
{
	unsignedInteger count = table.getCount();

	for (unsignedInteger i = 0; i < count; ++i)
	{
		if (table[i])
		{
			insert(*table[i]);
		}
	}
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::rehashCluster(unsignedInteger start)
{
	assert(start < tableSize);

	unsignedInteger positionToEmpty = start;

	while (table[positionToEmpty])
	{
		Item* extractedItem = extractItemFromTableAt(positionToEmpty);

		insert(*extractedItem);

		positionToEmpty = (positionToEmpty + 1) % tableSize;
	}
}


template <typename Item, typename Key, typename KeyAccessor>
inline Item* Hash<Item, Key, KeyAccessor>::extractItemFromTableAt(unsignedInteger index)
{
	assert(index < tableSize && table[index]);

	Item* result = table[index];

	table[index] = nullptr;
	--insertedCount;

	return result;
}


template <typename Item, typename Key, typename KeyAccessor>
void Hash<Item, Key, KeyAccessor>::empty()
{
	toEmptyStateOfSize(MIN_TABLE_SIZE);
}


//
// ( 3 * expectedSize ) / 2 is used because if all the expected items
// are inserted, the load factor will be 2/3 
// 
template <typename Item, typename Key, typename KeyAccessor>
typename Hash<Item,Key,KeyAccessor>::unsignedInteger Hash<Item, Key, KeyAccessor>::calculateAppropriateSize(unsignedInteger expectedCount)
{
	assert(expectedCount > 0);

	return (expectedCount < MIN_TABLE_SIZE) ? MIN_TABLE_SIZE : (3 * expectedCount) / 2;
}


template <typename Item, typename Key, typename KeyAccessor>
inline void Hash<Item, Key, KeyAccessor>::nullify(DArray<Item*>& table)
{
	const unsignedInteger count = table.getCount();

	for (unsignedInteger i = 0; i < count; ++i)
	{
		table[i] = nullptr;
	}
}

template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isEmpty() const
{
	return insertedCount == 0;
}


template <typename Item,typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::hasTooManyEmptySlots() const
{
	return 6 * insertedCount <= tableSize;
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::canBeShrinked() const
{
	return tableSize / GROWTH_FACTOR >= MIN_TABLE_SIZE;
}


template <typename Item, typename Key, typename KeyAccessor>
inline bool Hash<Item, Key, KeyAccessor>::isFillingUp() const
{
	return 3 * insertedCount >= 2 * tableSize;
}


template <typename Item, typename Key, typename KeyAccessor>
inline typename Hash<Item,Key,KeyAccessor>::unsignedInteger Hash<Item, Key, KeyAccessor>::getCount() const
{
	return insertedCount;
}

