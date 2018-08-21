

template <typename T>
template <typename Item, bool isConst>
inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::SinglyLinkedListIterator(nodePtr startNode, ownerPtr owner) :
	current{ startNode },
	owner{ owner }
{
}


template <typename T>
template <typename Item, bool isConst>
inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::SinglyLinkedListIterator(const SinglyLinkedListIterator<Item, false>& source) :
	SinglyLinkedListIterator<Item, isConst>{ source.current, source.owner }
{
}


template <typename T>
template <typename Item, bool isConst>
inline typename SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::reference
SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator*() const
{
	assert(this->operator bool());
	return current->data;
}


template <typename T>
template <typename Item, bool isConst>
inline typename SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::pointer
SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator->() const
{
	return &(this->operator*());
}


template <typename T>
template <typename Item, bool isConst>
typename SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>
SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator++(int)
{
	SinglyLinkedListIterator<Item, isConst> temporary{ *this };
	++(*this);

	return temporary;
}


template <typename T>
template <typename Item, bool isConst>
typename SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>& 
SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator++()
{
	if (this->operator bool())
	{
		current = current->next;
	}

	return *this;
}


template <typename T>
template <typename Item, bool isConst>
inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator bool() const
{
	return current != nullptr;
}


template <typename T>
template <typename Item, bool isConst>
inline bool SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator!() const
{
	return !(this->operator bool());
}


template <typename T>
template <typename Item, bool isConst>
inline typename SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::baseIteratorPtr
SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::clone() const
{
	return baseIteratorPtr{ new SinglyLinkedListIterator<Item, isConst>{*this} };
}


template <typename T, bool isConst>
inline bool operator==(typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& lhs,
	                   typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}


template <typename T, bool isConst>
inline bool operator!=(typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& lhs, 
	                   typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& rhs)
{
	return !(lhs == rhs);
}