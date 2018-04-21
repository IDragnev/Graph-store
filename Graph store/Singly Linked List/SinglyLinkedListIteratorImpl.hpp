
template <typename Key>
SinglyLinkedListIterator<Key>::SinglyLinkedListIterator(Node<Key>* startNode, const SinglyLinkedList<Key>* owner) :
	current(startNode),
	owner(owner)
{
}


template <typename Key>
inline SinglyLinkedListIterator<Key>::operator bool() const
{
	return current != nullptr;
}


template <typename Key>
inline bool SinglyLinkedListIterator<Key>::operator!() const
{
	return current == nullptr;
}


template <typename Key>
inline SinglyLinkedListIterator<Key>& SinglyLinkedListIterator<Key>::operator++()
{
	if (current != nullptr)
		current = current->next;

	return *this;
}


template <typename Key>
inline SinglyLinkedListIterator<Key> SinglyLinkedListIterator<Key>::operator++(int)
{
	SinglyLinkedListIterator<Key> temp(*this);

	++(*this);

	return temp;
}


template <typename Key>
inline Key& SinglyLinkedListIterator<Key>::operator*()
{
	return current->data;
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