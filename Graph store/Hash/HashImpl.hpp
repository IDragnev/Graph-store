

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




template <typename Item, typename Key, typename KeyExtractor>
int Hash<Item, Key, KeyExtractor>::getIndexByKey(const Key& key)
{
	const int size = table.getSize();

	//extract the key and calculate its hash value
	size_t hash = hashFunction(key) % size;

	while (table[hash] != nullptr && keyExtractor( *(table[hash]) ) != key)
	{
		hash = (hash + 1) % size;
	}

	//TODO 
	return -1; //HOW
}