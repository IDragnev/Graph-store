
template <typename Key, bool isConst>
SinglyLinkedListIterator<Key, isConst>::SinglyLinkedListIterator(nodePtr startNode, const SinglyLinkedList<Key>* owner) :
	current(startNode),
	owner(owner)
{
}


template <typename Key, bool isConst>
SinglyLinkedListIterator<Key, isConst>::SinglyLinkedListIterator(const SinglyLinkedListIterator<Key, false>& source) :
	current(source.current),
	owner(source.owner)
{
}


template <typename Key, bool isConst>
inline typename SinglyLinkedListIterator<Key, isConst>::reference SinglyLinkedListIterator<Key, isConst>::operator*() const
{
	return getCurrent();
}


template <typename Key, bool isConst>
inline SinglyLinkedListIterator<Key, isConst>& SinglyLinkedListIterator<Key, isConst>::operator++()
{
	goToNext();

	return *this;
}


template <typename Key, bool isConst>
inline SinglyLinkedListIterator<Key, isConst> SinglyLinkedListIterator<Key, isConst>::operator++(int)
{
	SinglyLinkedListIterator<Key, isConst> temporary(*this);

	++(*this);

	return temporary;
}


template <typename Key, bool isConst>
inline SinglyLinkedListIterator<Key, isConst>::operator bool() const
{
	return !isFinished();
}


template <typename Key, bool isConst>
inline bool SinglyLinkedListIterator<Key, isConst>::operator!() const
{
	return isFinished();
}


template <typename Key, bool isConst>
inline bool SinglyLinkedListIterator<Key, isConst>::isFinished() const
{
	return current == nullptr;
}


template <typename Key, bool isConst>
inline void SinglyLinkedListIterator<Key, isConst>::goToNext()
{
	if (!isFinished())
	{
		current = current->next;
	}
}


template <typename Key, bool isConst>
inline typename SinglyLinkedListIterator<Key, isConst>::reference SinglyLinkedListIterator<Key, isConst>::getCurrent() const
{
	return current->data;
}


template <typename Key, bool isConst>
inline typename SinglyLinkedListIterator<Key, isConst>::baseIterator* SinglyLinkedListIterator<Key, isConst>::clone() const
{
	return new SinglyLinkedListIterator<Key, isConst>(*this);
}


//template <typename Key, bool isConst>
//inline bool operator==(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs)
//{
//	return (lhs.current == rhs.current && lhs.owner == rhs.owner);
//}
//
//
//template <typename Key, bool isConst>
//inline bool operator!=(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs)
//{
//	return !(lhs == rhs);
//}