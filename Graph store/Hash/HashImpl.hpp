#include <algorithm>

/*
     MIN_TABLE_SIZE is 3 because it should be small enough to be rarely used in the constructor,
	 big enough to have empty slots for smaller counts (when the table holds 1 or 2 items),
	 and small enough to be able to halve small tables after removal (when size is 6, 7, 8)
*/


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Hash<Item, Key, KeyAccessor, Hasher>::Hash() :
	Hash{ MIN_TABLE_SIZE }
{
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
template <typename InputIt>
Hash<Item, Key, KeyAccessor, Hasher>::Hash(InputIt first, InputIt last) :
	Hash{ std::distance(first, last) }
{
	std::for_each(first, last, [&](Item& item) { insert(item); });
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Hash<Item, Key, KeyAccessor, Hasher>::Hash(size_type expectedCount)
{
	toEmptyStateOfSize(calculateAppropriateSize(expectedCount));
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::toEmptyStateOfSize(size_type size)
{
	assert(size >= MIN_TABLE_SIZE);
	table = Table(size, size);
	tableSize = size;
	insertedCount = 0;
	nullify(table);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::nullify(Table& table)
{
	for (auto&& slot : table)
	{
		slot = nullptr;
	}
}


//
// ( 3 * expectedSize ) / 2 is used because if all the expected items
// are inserted, the load factor will be 2/3 
// 
template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline typename Hash<Item, Key, KeyAccessor, Hasher>::size_type
Hash<Item, Key, KeyAccessor, Hasher>::calculateAppropriateSize(size_type expectedCount)
{
	assert(expectedCount > 0);
	return (expectedCount < MIN_TABLE_SIZE) ? MIN_TABLE_SIZE : (3 * expectedCount) / 2;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Hash<Item, Key, KeyAccessor, Hasher>::Hash(Hash&& source) :
	Hash{}
{
	std::swap(table, source.table);
	std::swap(tableSize, source.tableSize);
	std::swap(insertedCount, source.insertedCount);
	hashFunction = std::move(source.hashFunction);
	keyAccessor = std::move(source.keyAccessor);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Hash<Item, Key, KeyAccessor, Hasher>&
Hash<Item, Key, KeyAccessor, Hasher>::operator=(Hash&& other)
{
	if (this != &other)
	{
		swapContentsWithReconstructedParameter(std::move(other));
	}

	return *this;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Hash<Item, Key, KeyAccessor, Hasher>& 
Hash<Item, Key, KeyAccessor, Hasher>::operator=(const Hash& other)
{
	if (this != &other)
	{
		swapContentsWithReconstructedParameter(other);
	}

	return *this;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::swapContentsWithReconstructedParameter(Hash temporary)
{
	std::swap(tableSize, temporary.tableSize);
	std::swap(insertedCount, temporary.insertedCount);
	std::swap(table, temporary.table);
	std::swap(hashFunction, temporary.hashFunction);
	std::swap(keyAccessor, temporary.keyAccessor);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::insert(Item& item)
{
	if (isFillingUp())
	{
		enlarge();
	}

	auto index = computeHashValue(keyAccessor(item));

	while (table[index])
	{
		index = (index + 1) % tableSize;
	}

	table[index] = &item;
	++insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline std::size_t Hash<Item, Key, KeyAccessor, Hasher>::computeHashValue(const Key& key) const
{
	return hashFunction(key) % tableSize;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline bool Hash<Item, Key, KeyAccessor, Hasher>::isFillingUp() const
{
	return 3 * insertedCount >= 2 * tableSize;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline void Hash<Item, Key, KeyAccessor, Hasher>::enlarge()
{
	resize(tableSize * GROWTH_FACTOR);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::resize(size_type newSize)
{
	//must have at least one empty position after resize
	assert(newSize >= MIN_TABLE_SIZE && newSize > insertedCount);
	
	Table oldTable{ std::move(table) };
	
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


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::insertAllItemsFrom(Table& table)
{
	for (auto&& itemPtr : table)
	{
		if (itemPtr)
		{
			insert(*itemPtr);
		}
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline const Item* Hash<Item, Key, KeyAccessor, Hasher>::search(const Key& key) const
{
	const long index = getPositionOfItemWithKey(key);
	return index >= 0 ? table[index] : nullptr;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline Item* Hash<Item, Key, KeyAccessor, Hasher>::search(const Key& key)
{
	return const_cast<Item*>( static_cast<const Hash&>(*this).search(key) );
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
long Hash<Item, Key, KeyAccessor, Hasher>::getPositionOfItemWithKey(const Key& key) const
{
	auto index = computeHashValue(key);

	while (table[index])
	{
		if (keyAccessor( *(table[index]) ) == key)
		{
			return index;
		}

		index = (index + 1) % tableSize;
	}

	return -1;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Item* Hash<Item, Key, KeyAccessor, Hasher>::remove(const Key& key)
{
	auto index = getPositionOfItemWithKey(key);

	if (index >= 0)
	{
		auto* result = extractItemFromTableAt(index);

		if (hasTooManyEmptySlots() && canBeShrinked())
		{
			shrink();
		}
		else
		{
			rehashCluster((index + 1) % tableSize);
		}
		
		return result;
	}

	return nullptr;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline void Hash<Item, Key, KeyAccessor, Hasher>::shrink()
{
	resize(tableSize / GROWTH_FACTOR);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Item* Hash<Item, Key, KeyAccessor, Hasher>::extractItemFromTableAt(size_type index)
{
	assert(index < tableSize && table[index]);

	auto* result = table[index];
	table[index] = nullptr;
	--insertedCount;

	return result;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline bool Hash<Item, Key, KeyAccessor, Hasher>::hasTooManyEmptySlots() const
{
	return 6 * insertedCount <= tableSize;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline bool Hash<Item, Key, KeyAccessor, Hasher>::canBeShrinked() const
{
	return tableSize / GROWTH_FACTOR >= MIN_TABLE_SIZE;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::rehashCluster(size_type start)
{
	assert(start < tableSize);

	auto positionToEmpty = start;

	while (table[positionToEmpty])
	{
		auto* extracted = extractItemFromTableAt(positionToEmpty);
		insert(*extracted);

		positionToEmpty = (positionToEmpty + 1) % tableSize;
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline void Hash<Item, Key, KeyAccessor, Hasher>::empty()
{
	toEmptyStateOfSize(MIN_TABLE_SIZE);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline bool Hash<Item, Key, KeyAccessor, Hasher>::isEmpty() const
{
	return insertedCount == 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline typename Hash<Item,Key,KeyAccessor, Hasher>::size_type 
Hash<Item, Key, KeyAccessor, Hasher>::getCount() const
{
	return insertedCount;
}

