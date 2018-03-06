
/*
     MIN_SIZE is 3 because it should be small enough to be rarely used in the constructor,
	 big enough to have empty boxes for smaller counts ( when the table holds 1 or 2 items ),
	 and small enough to be able to halve small tables after removal ( when size is 6,7,8 )
*/


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
	return count + 1 == table.getSize();
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
	const int tableSize = table.getSize();

	int hash = hashFunction(key) % tableSize;

	while (table[hash] != nullptr)
	{
		if (keyExtractor( *(table[hash]) ) == key)
			return hash;

		hash = (hash + 1) % tableSize;
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
template <typename Item, typename Key, typename KeyExtractor>
Item* Hash<Item, Key, KeyExtractor>::search(const Key& key)
{
	const int pos = getIndexByKey(key);

	assert(pos < table.getSize());

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
	const int tableSize = table.getSize();

	for (int i = 0; i < tableSize; ++i)
		table[i] = nullptr;
}



//
//make a new table with the sent size and rehash all items in it
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::resize(int newSize)
{
	//must have at least one empty pos. after resize
	assert(newSize >= MIN_SIZE && newSize > count);
	
	const int oldSize = table.getSize();

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
//shrink the table to minimum size, null pointers and count
//
// (!) all registered objects' addresses are lost, the caller is responsible
//     for saving them elsewhere
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::empty()
{
	table.shrink(MIN_SIZE);

	nullTable();

	count = 0;
}



//
//remove an item from the table by its key and return its address to the caller
//
// \ if there is no item with such key, the table is not modified and nullptr is returned
//
// \ else the item is removed from the table and:
//    - if the table is at most 1/6 Full after the removal AND its size could be halved (without going below MIN_SIZE),
//      it is halved and all the items are rehashed
//    - else all the items after it in the same cluster are rehashed  
//
template <typename Item, typename Key, typename KeyExtractor>
Item* Hash<Item, Key, KeyExtractor>::remove(const Key& key)
{
	const int pos = getIndexByKey(key);

	if (pos >= 0)
	{
		const int tableSize = table.getSize();

		assert(pos < tableSize);

		Item* result =  table[pos];
		assert(result);

		table[pos] = nullptr;
		--count; 

		if (6 * count <= tableSize && tableSize / 2 >= MIN_SIZE)
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
	const int tableSize = table.getSize();

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




//
//insert an item in the hash table using its key
//
// \ if the table is at least 2/3 full, it is doubled
//
template <typename Item, typename Key, typename KeyExtractor>
void Hash<Item, Key, KeyExtractor>::insert(Item& item)
{
	int tableSize = table.getSize();

	if (3 * count >= 2 * tableSize)
		resize( tableSize *= 2 );

	int hash = hashFunction( keyExtractor(item) ) % tableSize;

	while(table[hash] != nullptr)
		hash = (hash + 1) % tableSize;

	table[hash] = &item;
	++count;
}




//
//construct the table and null it
//
// \ if the expected size is less than the minimum size, the table holds
//   MIN_SIZE number of pointers ( MIN_SIZE will be small )
//
// \ else ( 3 * expectedSize ) / 2 is used, this way if all the expected 
//   items are inserted, the table will be 2/3 full and will work nicely
//
// \ if the expected size is not positive, std::invalid_arg is thrown
//
// (!) MIN_SIZE is positive, so the table constructor will never throw an exception
//
template <typename Item, typename Key, typename KeyExtractor>
Hash<Item, Key, KeyExtractor>::Hash(int expectedSize)
	:
	count(0),
	table(expectedSize < MIN_SIZE ? MIN_SIZE : (3 * expectedSize) / 2)     
{
	if (expectedSize <= 0)
		throw std::invalid_argument("Expected size must be positive!");

	nullTable();
}