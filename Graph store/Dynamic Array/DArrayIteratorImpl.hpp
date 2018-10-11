
template <typename T>
template <typename Item, bool isConst>
inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(typename DArray<T>::size_type startPosition, ownerPtr owner) :
	current{ startPosition },
	owner{ owner }
{
}


template <typename T>
template <typename Item, bool isConst>
inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(const DArrayIterator<Item, false>& source) :
	DArrayIterator<Item, isConst>{ source.current, source.owner }
{
}


template <typename T>
template <typename Item, bool isConst>
inline auto DArray<T>::DArrayIterator<Item, isConst>::operator*() const -> reference
{
	assert(this->operator bool());
	return (*owner)[current];
}


template <typename T>
template <typename Item, bool isConst>
inline auto DArray<T>::DArrayIterator<Item, isConst>::operator->() const -> pointer
{
	return &(this->operator*());
}


template <typename T>
template <typename Item, bool isConst>
auto DArray<T>::DArrayIterator<Item, isConst>::operator++(int) -> DArrayIterator
{
	auto temp = *this;
	++(*this);

	return temp;
}


template <typename T>
template <typename Item, bool isConst>
inline auto DArray<T>::DArrayIterator<Item, isConst>::operator++() -> DArrayIterator&
{
	++current;
	return *this;
}


template <typename T>
template <typename Item, bool isConst>
inline DArray<T>::DArrayIterator<Item, isConst>::operator bool() const
{
	return current < owner->getCount();
}


template <typename T>
template <typename Item, bool isConst>
inline bool DArray<T>::DArrayIterator<Item, isConst>::operator!() const
{
	return !(this->operator bool());
}


template <typename Item, bool isConst>
inline bool operator==(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs,
					   typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}


template <typename Item, bool isConst>
inline bool operator!=(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs, 
					   typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs)
{
	return !(lhs == rhs);
}
