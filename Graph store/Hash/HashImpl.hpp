

template <typename Item, typename Key, typename KeyExtractor>
inline int Hash<Item, Key, KeyExtractor>::getCount()const
{
	return count;
}


template <typename Item, typename Key, typename KeyExtractor>
inline bool Hash<Item, Key, KeyExtractor>::isEmpty()const
{
	return count == 0;
}



//
//the hash is full when there is exactly one vacant position left
//in the table
//
template <typename Item, typename Key, typename KeyExtractor>
inline bool Hash<Item, Key, KeyExtractor>::isFull()const
{
	return count + 1 == table.getCount();
}



//
//search the table until an empty position or the sought item is found
//(going to the beginning if the end is reached)
//
// \ if the item is found, its index is returned 
// 
// \ if there is no item with such key, -1 is returned
//
// (!) the function is not const because keyExtractor and hashFunction
//   could have non-const operator()
//
template <typename Item, typename Key, typename KeyExtractor>
int Hash<Item, Key, KeyExtractor>::getIndexByKey(const Key& key)
{
	const int size = table.getCount();

	int hash = hashFunction(key) % size;

	while (table[hash] != nullptr)
	{
		if (keyExtractor( *(table[hash]) ) == key)
			return hash;

		hash = (hash + 1) % size;
	}

	return -1;
}




//
//remove an item from the table by its key and return its address to the caller
//
// \ if there is no item with such key, the table is not modified and nullptr is returned
//
template <typename Item, typename Key, typename KeyExtractor>
Item* Hash<Item, Key, KeyExtractor>::remove(const Key& key)
{
	const int pos = getIndexByKey(key);

	if (pos >= 0)
	{
		Item* result =  table[pos];

		assert(result);

		table[pos] = nullptr;

		//REHASH ALL AFTER IT....................................................................................................
		
		//SHRINK IF..............................................................................................................

		return result;
	}

	return nullptr;
}




//
//search for an item by a key
//
// \ if the item is found, its address is returned
//
// \ if there is no item with such key, nullptr is returned
//
// (!) the function is not const because getIndexByKey is not const
//
template <typename Item, typename Key, typename KeyExtractor>
Item* Hash<Item, Key, KeyExtractor>::search(const Key& key)
{
	const int pos = getIndexByKey(key);

	if (pos >= 0)
		return table[pos];
	else
		return nullptr;
}




//
//null all pointers in the table (count is not touched)
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::nullTable()
{
	const int size = table.getCount();

	for (int i = 0; i < size; ++i)
		table[i] = nullptr;
}




template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::resize(int newSize)
{
	//must have at least one empty pos. after resize
	assert(newSize > 0 && newSize > count);
	
	const int oldSize = table.getCount();

	DArray<Item*> temp(newSize, newSize);

	//after the swap the table holds random pointers
	std::swap(table, temp);
	
	nullTable();

	//rehash items
	for (int i = 0; i < oldSize; ++i)
	{
		if (temp[i] != nullptr)
			insert(*temp[i]);
	}
}




//
//possibly free the table memory (do not depend on DArray impl.)
//and update count 
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::empty()
{
	table.shrink(0);

	assert(table.getCount() == 0);

	count = 0;
}