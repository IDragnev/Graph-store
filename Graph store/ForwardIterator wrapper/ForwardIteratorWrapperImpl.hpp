

template <typename ForwardIterator>
ForwardIteratorWrapper<ForwardIterator>::ForwardIteratorWrapper(const ForwardIterator& it) :
	iterator{ it }
{
}


template <typename ForwardIterator>
typename ForwardIteratorWrapper<ForwardIterator>::reference
ForwardIteratorWrapper<ForwardIterator>::operator*() const
{
	return *iterator;
}


template <typename ForwardIterator>
typename ForwardIteratorWrapper<ForwardIterator>::pointer
ForwardIteratorWrapper<ForwardIterator>::operator->() const
{
	return iterator.operator->();
}


template <typename ForwardIterator>
ForwardIteratorWrapper<ForwardIterator>
ForwardIteratorWrapper<ForwardIterator>::operator++(int)
{
	ForwardIteratorWrapper temp{ *this };
	++(*this);

	return temp;
}


template <typename ForwardIterator>
ForwardIteratorWrapper<ForwardIterator>&
ForwardIteratorWrapper<ForwardIterator>::operator++()
{
	++iterator;
	return *this;
}


template <typename ForwardIterator>
ForwardIteratorWrapper<ForwardIterator>::operator bool() const
{
	return static_cast<bool>(iterator);
}


template <typename ForwardIterator>
bool ForwardIteratorWrapper<ForwardIterator>::operator!() const
{
	return iterator.operator!();
}


template <typename ForwardIterator>
typename ForwardIteratorWrapper<ForwardIterator>::baseIteratorPtr
ForwardIteratorWrapper<ForwardIterator>::clone() const
{
	return baseIteratorPtr{ new ForwardIteratorWrapper{ *this } };
}