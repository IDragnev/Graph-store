
template <typename T>
template <typename Item, bool isConst>
inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(typename DArray<T>::unsignedInteger startPosition, ownerPtr owner) :
	current{ startPosition },
	owner{ owner }
{
}


template <typename T>
template <typename Item, bool isConst>
inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(const DArrayIterator<Item, false>& source) :
	current{ source.current },
	owner{ source.owner }
{
}


template <typename T>
template <typename Item, bool isConst>
inline typename DArray<T>::DArrayIterator<Item, isConst>::reference 
DArray<T>::DArrayIterator<Item, isConst>::operator*() const
{
	return getCurrent();
}


template <typename T>
template <typename Item, bool isConst>
inline typename DArray<T>::DArrayIterator<Item, isConst>::pointer
DArray<T>::DArrayIterator<Item, isConst>::operator->() const
{
	return &getCurrent();
}


template <typename T>
template <typename Item, bool isConst>
inline typename DArray<T>::DArrayIterator<Item, isConst>::reference 
DArray<T>::DArrayIterator<Item, isConst>::getCurrent() const
{
	assert(!isFinished());
	return (*owner)[current];
}


template <typename T>
template <typename Item, bool isConst>
typename DArray<T>::DArrayIterator<Item, isConst>
DArray<T>::DArrayIterator<Item, isConst>::operator++(int)
{
	DArrayIterator<Item, isConst> temporary{ *this };
	++(*this);

	return temporary;
}


template <typename T>
template <typename Item, bool isConst>
typename DArray<T>::DArrayIterator<Item, isConst>& 
DArray<T>::DArrayIterator<Item, isConst>::operator++()
{
	goToNext();

	return *this;
}


template <typename T>
template <typename Item, bool isConst>
inline void DArray<T>::DArrayIterator<Item, isConst>::goToNext()
{
	if (!isFinished())
	{
		++current;
	}
}


template <typename T>
template <typename Item, bool isConst>
inline DArray<T>::DArrayIterator<Item, isConst>::operator bool() const
{
	return !isFinished();
}


template <typename T>
template <typename Item, bool isConst>
inline bool DArray<T>::DArrayIterator<Item, isConst>::operator!() const
{
	return isFinished();
}


template <typename T>
template <typename Item, bool isConst>
inline bool DArray<T>::DArrayIterator<Item, isConst>::isFinished() const
{
	return current >= owner->getCount();
}


template <typename T>
template <typename Item, bool isConst>
inline typename DArray<T>::DArrayIterator<Item, isConst>::baseIteratorPtr 
DArray<T>::DArrayIterator<Item, isConst>::clone() const
{
	return baseIteratorPtr{ new DArrayIterator<Item, isConst>{*this} };
}



