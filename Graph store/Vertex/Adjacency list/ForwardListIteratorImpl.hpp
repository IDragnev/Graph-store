

template <typename Key>
ForwardListIterator<Key>::ForwardListIterator(Node<Key>* startNode, const ForwardList<Key>* owner)
	:
	current(startNode),
	owner(owner)
{
	;
}


//
//returns true if the traversal has not ended
//
template <typename Key>
ForwardListIterator<Key>::operator bool()const
{
	return current != nullptr;
}



//
//returns true if the traversal has ended
//
template <typename Key>
bool ForwardListIterator<Key>::operator!()const
{
	return current == nullptr;
}



//
//advance to the next node in the list (if not null)
//
template <typename Key>
ForwardListIterator<Key>& ForwardListIterator<Key>::operator++()
{
	if (current)
		current = current->next;

	return *this;
}


template <typename Key>
ForwardListIterator<Key> ForwardListIterator<Key>::operator++(int)
{
	ForwardListIterator<Key> temp(*this);

	++(*this);

	return temp;
}


//
//return the data of the current node
//
template <typename Key>
Key& ForwardListIterator<Key>::operator*()
{
	return current->data;
}



//
//owner and current should be the same
//
template <typename Key>
bool operator==(const ForwardListIterator<Key>& lhs, const ForwardListIterator<Key>& rhs)
{
	return (lhs.current == rhs.current && lhs.owner == rhs.owner);
}


template <typename Key>
bool operator!=(const ForwardListIterator<Key>& lhs, const ForwardListIterator<Key>& rhs)
{
	return !(lhs == rhs);
}