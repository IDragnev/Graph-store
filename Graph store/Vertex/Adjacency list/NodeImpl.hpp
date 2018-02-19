

template <typename Key>
Node<Key>::Node(const Key& data, Node<Key>* next)
	:
	next(next),
	data(data)
{
	;
}


template <typename Key>
Node<Key>::Node(Key&& data, Node<Key>* next)
	:
	next(next),
	data(std::move(data))
{
	;
}


template <typename Key>
bool Node<Key>::hasSuccessor()const
{
	return next != nullptr;
}
