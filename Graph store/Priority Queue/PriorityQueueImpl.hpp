

template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, CompareFunction, HandleSetter>::PriorityQueue() :
	elements(),
	insertedCount(0)
{
}


template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
bool PriorityQueue<Item, Key, CompareFunction, HandleSetter>::isEmpty() const
{
	return insertedCount == 0;
}

