
template <typename Key>
SinglyLinkedListIterator<Key>::SinglyLinkedListIterator(Node<Key>* startNode, const SinglyLinkedList<Key>* owner) :
	current(startNode),
	owner(owner)
{
}


template <typename Key>
Key& SinglyLinkedListIterator<Key>::operator*()
{
	return getCurrent();
}


template <typename Key>
SinglyLinkedListIterator<Key>& SinglyLinkedListIterator<Key>::operator++()
{
	goToNext();

	return *this;
}


template <typename Key>
SinglyLinkedListIterator<Key> SinglyLinkedListIterator<Key>::operator++(int)
{
	SinglyLinkedListIterator<Key> temporary(*this);

	++(*this);

	return temporary;
}


template <typename Key>
SinglyLinkedListIterator<Key>::operator bool() const
{
	return !isFinished();
}


template <typename Key>
bool SinglyLinkedListIterator<Key>::operator!() const
{
	return isFinished();
}


template <typename Key>
inline bool SinglyLinkedListIterator<Key>::isFinished() const
{
	return current == nullptr;
}


template <typename Key>
inline void SinglyLinkedListIterator<Key>::goToNext()
{
	if (!isFinished())
		current = current->next;
}


template <typename Key>
inline Key& SinglyLinkedListIterator<Key>::getCurrent()
{
	return current->data;
}


template <typename Key>
inline Iterator<Key>* SinglyLinkedListIterator<Key>::clone() const
{
	return new SinglyLinkedListIterator<Key>(*this);
}


template <typename Key>
inline bool operator==(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs)
{
	return (lhs.current == rhs.current && lhs.owner == rhs.owner);
}


template <typename Key>
inline bool operator!=(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs)
{
	return !(lhs == rhs);
}