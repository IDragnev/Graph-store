
template <typename T>
inline bool Queue<T>::isEmpty() const
{
	return theUnderlyingList.isEmpty();
}


template <typename T>
inline void Queue<T>::empty() 
{
	theUnderlyingList.empty();
}


template <typename T>
inline void Queue<T>::enqueue(const T& item)
{
	theUnderlyingList.insertAsTail(item);
}


template <typename T>
inline T Queue<T>::dequeue()
{
	T head(peekHead());
	
	theUnderlyingList.removeHead();

	return head;
}


template <typename T>
inline T Queue<T>::peekHead() const
{
	assert(!isEmpty());

	return theUnderlyingList.getHead();
}