

template <typename Item, typename Key>
Pair<Item, Key>::Pair() :
	itemPtr(nullptr),
	key()
{
}


template <typename Item, typename Key>
Pair<Item, Key>::Pair(Item* itemPtr, const Key& key) :
	itemPtr(itemPtr),
	key(key)
{
}


template <typename Item, typename Key>
Pair<Item, Key>::Pair(Item* itemPtr, Key&& key) :
	itemPtr(itemPtr),
	key(std::move(key))
{
}