

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
// (!!) the function handles the case where table.getCount() == 0
//
template <typename Item, typename Key, typename KeyExtractor>
int Hash<Item, Key, KeyExtractor>::getIndexByKey(const Key& key)
{
	if (!isEmpty()) // (!!) size = 0 -> count = 0 , so it will handle this case too
	{
		const int size = table.getCount();

		int hash = hashFunction(key) % size; // (!!) size != 0 

		while (table[hash] != nullptr)
		{
			if (keyExtractor( *(table[hash]) ) == key)
				return hash;

			hash = (hash + 1) % size;
		}

	}

	return -1;
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
// (!!) getIndexByKey handles the case where table.getCount() == 0 (table.size == 0)
//
template <typename Item, typename Key, typename KeyExtractor>
Item* Hash<Item, Key, KeyExtractor>::search(const Key& key)
{
	const int pos = getIndexByKey(key);

	assert(pos < table.getCount());

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



//
//make a new table with the sent size and rehash all items in it
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::resize(int newSize)
{
	//must have at least one empty pos. after resize
	assert(newSize - count > 0);
	
	const int oldSize = table.getCount();

	DArray<Item*> temp(newSize, newSize);

	//after the swap the table holds randomly initialized pointers
	//and temp holds currently registered objects
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
// (!) all registered objects' addresses are lost, the caller is responsible
//     for saving them elsewhere
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::empty()
{
	table.shrink(0);

	assert(table.getCount() == 0);

	count = 0;
}



//
//remove an item from the table by its key and return its address to the caller
//
// \ if there is no item with such key, the table is not modified and nullptr is returned
//
// \ else the item is removed from the table and:
//    - if the table is 1/6 Full after the removal, it is halved and all the items are rehashed
//    - else all the items after it in the same cluster are rehashed  
//
// (!) getIndexByKey handles the case where table.getCount() == 0 (table.size == 0)
//
template <typename Item, typename Key, typename KeyExtractor>
Item* Hash<Item, Key, KeyExtractor>::remove(const Key& key)
{
	const int pos = getIndexByKey(key);

	if (pos >= 0)
	{
		const int tableSize = table.getCount();

		assert(pos < tableSize);

		Item* result =  table[pos];
		assert(result);

		table[pos] = nullptr;
		--count; 

		if (6 * count <= tableSize)
			resize(tableSize / 2);
		else
			rehashCluster( (pos + 1) % tableSize );
		
		return result;
	}

	return nullptr;
}




//
//rehash all the items from the cluster whose start is passed
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::rehashCluster(int start)
{
	const int tableSize = table.getCount();

	Item* current = table[start];

	//until the end of the cluster is reached
	while (current != nullptr)
	{
		//remove current from the table
		table[start] = nullptr;
		--count;

		insert(*current);

		start = (start + 1) % tableSize;
		current = table[start];
	}
}
