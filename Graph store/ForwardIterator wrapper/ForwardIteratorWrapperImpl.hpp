

template <typename ForwardIterator, typename EndType>
ForwardIteratorWrapper<ForwardIterator, EndType>::ForwardIteratorWrapper(const ForwardIterator& it, const EndType& end) :
	iterator{ it },
	end{ end }
{
}


template <typename ForwardIterator, typename EndType>
typename ForwardIteratorWrapper<ForwardIterator, EndType>::reference
ForwardIteratorWrapper<ForwardIterator, EndType>::operator*() const
{
	return *iterator;
}


template <typename ForwardIterator, typename EndType>
typename ForwardIteratorWrapper<ForwardIterator, EndType>::pointer
ForwardIteratorWrapper<ForwardIterator, EndType>::operator->() const
{
	return iterator.operator->();
}


template <typename ForwardIterator, typename EndType>
ForwardIteratorWrapper<ForwardIterator, EndType>
ForwardIteratorWrapper<ForwardIterator, EndType>::operator++(int)
{
	ForwardIteratorWrapper temp{ *this };
	++(*this);

	return temp;
}


template <typename ForwardIterator, typename EndType>
ForwardIteratorWrapper<ForwardIterator, EndType>&
ForwardIteratorWrapper<ForwardIterator, EndType>::operator++()
{
	++iterator;
	return *this;
}


template <typename ForwardIterator, typename EndType>
ForwardIteratorWrapper<ForwardIterator, EndType>::operator bool() const
{
	return iterator != end;
}


template <typename ForwardIterator, typename EndType>
bool ForwardIteratorWrapper<ForwardIterator, EndType>::operator!() const
{
	return !(this->operator bool());
}


template <typename ForwardIterator, typename EndType>
typename ForwardIteratorWrapper<ForwardIterator, EndType>::baseIteratorPtr
ForwardIteratorWrapper<ForwardIterator, EndType>::clone() const
{
	return baseIteratorPtr{ new ForwardIteratorWrapper{ *this } };
}