

template <typename ForwardIterator, bool isConst>
ForwardIteratorWrapper<ForwardIterator, isConst>::ForwardIteratorWrapper(const ForwardIterator& it) :
	iterator{ it }
{
}


template <typename ForwardIterator, bool isConst>
ForwardIteratorWrapper<ForwardIterator, isConst>::ForwardIteratorWrapper(const ForwardIteratorWrapper<ForwardIterator, false>& source) :
	iterator{ source.iterator }
{
}


template <typename ForwardIterator, bool isConst>
typename ForwardIteratorWrapper<ForwardIterator, isConst>::reference
ForwardIteratorWrapper<ForwardIterator, isConst>::operator*() const
{
	return *iterator;
}


template <typename ForwardIterator, bool isConst>
typename ForwardIteratorWrapper<ForwardIterator, isConst>::pointer
ForwardIteratorWrapper<ForwardIterator, isConst>::operator->() const
{
	return iterator.operator->();
}


template <typename ForwardIterator, bool isConst>
ForwardIteratorWrapper<ForwardIterator, isConst>
ForwardIteratorWrapper<ForwardIterator, isConst>::operator++(int)
{
	ForwardIteratorWrapper temp{ *this };
	++(*this);

	return temp;
}


template <typename ForwardIterator, bool isConst>
ForwardIteratorWrapper<ForwardIterator, isConst>&
ForwardIteratorWrapper<ForwardIterator, isConst>::operator++()
{
	++iterator;
	return *this;
}


template <typename ForwardIterator, bool isConst>
ForwardIteratorWrapper<ForwardIterator, isConst>::operator bool() const
{
	return static_cast<bool>(iterator);
}


template <typename ForwardIterator, bool isConst>
bool ForwardIteratorWrapper<ForwardIterator, isConst>::operator!() const
{
	return iterator.operator!();
}


template <typename ForwardIterator, bool isConst>
typename ForwardIteratorWrapper<ForwardIterator, isConst>::baseIteratorPtr
ForwardIteratorWrapper<ForwardIterator, isConst>::clone() const
{
	return baseIteratorPtr{ new ForwardIteratorWrapper{ *this } };
}