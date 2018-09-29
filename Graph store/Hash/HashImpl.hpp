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
Hash<Item, Key, KeyAccessor, Hasher>::Hash(std::size_t expectedCount)
{
	toEmptyStateOfSize(calculateAppropriateSize(expectedCount));
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
void Hash<Item, Key, KeyAccessor, Hasher>::toEmptyStateOfSize(std::size_t size)
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
	for (auto* entry : table)
	{
		entry = nullptr;
	}
}


//
// ( 3 * expectedSize ) / 2 is used because if all the expected items
// are inserted, the load factor will be 2/3 
// 
template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline std::size_t
Hash<Item, Key, KeyAccessor, Hasher>::calculateAppropriateSize(std::size_t expectedCount)
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

	auto slot = computeHashValue(keyAccessor(item));
	auto emptySlot = findFirstEmptySlotStartingAt(slot);
	insertAt(emptySlot, item);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline std::size_t Hash<Item, Key, KeyAccessor, Hasher>::computeHashValue(const Key& key) const
{
	return hashFunction(key) % tableSize;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
std::size_t Hash<Item, Key, KeyAccessor, Hasher>::findFirstEmptySlotStartingAt(std::size_t slot) const
{
	while (!isEmpty(slot))
	{
		slot = followingSlot(slot);
	}

	return slot;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline void Hash<Item, Key, KeyAccessor, Hasher>::insertAt(std::size_t slot, Item& item)
{
	table[slot] = &item;
	++insertedCount;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline bool Hash<Item, Key, KeyAccessor, Hasher>::isEmpty(std::size_t slot) const
{
	return table[slot] == nullptr;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline std::size_t Hash<Item, Key, KeyAccessor, Hasher>::followingSlot(std::size_t slot) const
{
	return (slot + 1) % table.getSize();
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
void Hash<Item, Key, KeyAccessor, Hasher>::resize(std::size_t newSize)
{
	//must have at least one empty position after resize
	assert(newSize >= MIN_TABLE_SIZE && newSize > insertedCount);
	
	auto oldTable = Table{ std::move(table) };
	
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
	for (auto* entry : table)
	{
		if (entry)
		{
			insert(*entry);
		}
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline const Item* Hash<Item, Key, KeyAccessor, Hasher>::search(const Key& key) const
{
	long slot = correspondingSlot(key); /* asSigned(...) or a custom Result struct? */
	return slot >= 0 ? table[slot] : nullptr;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline Item* Hash<Item, Key, KeyAccessor, Hasher>::search(const Key& key)
{
	return const_cast<Item*>( static_cast<const Hash&>(*this).search(key) );
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
long Hash<Item, Key, KeyAccessor, Hasher>::correspondingSlot(const Key& key) const
{
	auto slot = computeHashValue(key);

	while (!isEmpty(slot))
	{
		if (keyAccessor( *(table[slot]) ) == key)
		{
			return slot;
		}

		slot = followingSlot(slot);
	}

	return -1;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Item* Hash<Item, Key, KeyAccessor, Hasher>::remove(const Key& key)
{
	auto slot = correspondingSlot(key);

	if (slot >= 0)
	{
		auto entry = extractSlotEntry(slot);

		if (hasTooManyEmptySlots() && canBeShrinked())
		{
			shrink();
		}
		else
		{
			rehashCluster(followingSlot(slot));
		}
		
		return entry;
	}

	return nullptr;
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
inline void Hash<Item, Key, KeyAccessor, Hasher>::shrink()
{
	resize(tableSize / GROWTH_FACTOR);
}


template <typename Item, typename Key, typename KeyAccessor, typename Hasher>
Item* Hash<Item, Key, KeyAccessor, Hasher>::extractSlotEntry(std::size_t slot)
{
	assert(slot < tableSize && !isEmpty(slot));

	auto entry = table[slot];
	table[slot] = nullptr;
	--insertedCount;

	return entry;
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
void Hash<Item, Key, KeyAccessor, Hasher>::rehashCluster(std::size_t startingSlot)
{
	assert(startingSlot < tableSize);

	auto slotToEmpty = startingSlot;

	while (!isEmpty(slotToEmpty))
	{
		auto* entry = extractSlotEntry(slotToEmpty);
		insert(*entry);

		slotToEmpty = followingSlot(slotToEmpty);
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
inline std::size_t Hash<Item, Key, KeyAccessor, Hasher>::getCount() const
{
	return insertedCount;
}

