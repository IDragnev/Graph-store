

template <typename ForwardIterator, typename EndType>
ForwardIteratorWrapper<ForwardIterator, EndType>::ForwardIteratorWrapper(const ForwardIterator& it, const EndType& end) :
	iterator{ it },
	end{ end }
{
}


template <typename ForwardIterator, typename EndType>
auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator*() const -> reference
{
	return *iterator;
}


template <typename ForwardIterator, typename EndType>
auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator->() const -> pointer
{
	return iterator.operator->();
}


template <typename ForwardIterator, typename EndType>
auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator++(int) -> ForwardIteratorWrapper
{
	auto temp = ForwardIteratorWrapper{ *this };
	++(*this);

	return temp;
}


template <typename ForwardIterator, typename EndType>
auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator++() -> ForwardIteratorWrapper&
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
auto ForwardIteratorWrapper<ForwardIterator, EndType>::clone() const -> BaseIteratorPtr
{
	return makeWrapper(iterator, end);
}