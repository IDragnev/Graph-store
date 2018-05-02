
template <typename T>
template <typename Item>
DArray<T>::DArrayIterator<Item>::DArrayIterator(typename DArray<T>::sizeType startPosition, DArray<Item>* owner) :
	current(startPosition),
	owner(owner)
{
}


template <typename T>
template <typename Item>
inline Item& DArray<T>::DArrayIterator<Item>::operator*()
{
	return getCurrent();
}


template <typename T>
template <typename Item>
inline typename DArray<T>::DArrayIterator<Item>& DArray<T>::DArrayIterator<Item>::operator++()
{
	goToNext();

	return *this;
}


template <typename T>
template <typename Item>
inline typename DArray<T>::DArrayIterator<Item> DArray<T>::DArrayIterator<Item>::operator++(int)
{
	DArrayIterator<Item> temporary(*this);

	++(*this);

	return temporary;
}


template <typename T>
template <typename Item>
inline DArray<T>::DArrayIterator<Item>::operator bool() const
{
	return !isFinished();
}


template <typename T>
template <typename Item>
inline bool DArray<T>::DArrayIterator<Item>::operator!() const
{
	return isFinished();
}


template <typename T>
template <typename Item>
inline void DArray<T>::DArrayIterator<Item>::goToNext()
{
	if (!isFinished())
		++current;
}


template <typename T>
template <typename Item>
inline Item& DArray<T>::DArrayIterator<Item>::getCurrent()
{
	return (*owner)[current];
}


template <typename T>
template <typename Item>
inline bool DArray<T>::DArrayIterator<Item>::isFinished() const
{
	return current >= owner->count;
}


template <typename T>
template <typename Item>
inline Iterator<Item>* DArray<T>::DArrayIterator<Item>::clone() const
{
	return new DArrayIterator<Item>(*this);
}



