
template <typename Key>
SinglyLinkedListIterator<Key>::SinglyLinkedListIterator(Node<Key>* startNode, const SinglyLinkedList<Key>* owner) :
	current(startNode),
	owner(owner)
{
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
inline SinglyLinkedListIterator<Key>* SinglyLinkedListIterator<Key>::clone() const
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