#include <assert.h>

template <typename Key, bool isConst>
inline SinglyLinkedListIterator<Key, isConst>::SinglyLinkedListIterator(nodePtr startNode, ownerPtr owner) :
	current{ startNode },
	owner{ owner }
{
}


template <typename Key, bool isConst>
inline SinglyLinkedListIterator<Key, isConst>::SinglyLinkedListIterator(const SinglyLinkedListIterator<Key, false>& source) :
	SinglyLinkedListIterator<Key, isConst>{ source.current, source.owner }
{
}


template <typename Key, bool isConst>
inline typename SinglyLinkedListIterator<Key, isConst>::reference 
SinglyLinkedListIterator<Key, isConst>::operator*() const
{
	assert(this->operator bool());
	return current->data;
}


template <typename Key, bool isConst>
inline typename SinglyLinkedListIterator<Key, isConst>::pointer
SinglyLinkedListIterator<Key, isConst>::operator->() const
{
	return &(this->operator*());
}


template <typename Key, bool isConst>
SinglyLinkedListIterator<Key, isConst> SinglyLinkedListIterator<Key, isConst>::operator++(int)
{
	SinglyLinkedListIterator<Key, isConst> temporary{ *this };
	++(*this);

	return temporary;
}


template <typename Key, bool isConst>
SinglyLinkedListIterator<Key, isConst>& SinglyLinkedListIterator<Key, isConst>::operator++()
{
	if (this->operator bool())
	{
		current = current->next;
	}

	return *this;
}


template <typename Key, bool isConst>
inline SinglyLinkedListIterator<Key, isConst>::operator bool() const
{
	return current != nullptr;
}


template <typename Key, bool isConst>
inline bool SinglyLinkedListIterator<Key, isConst>::operator!() const
{
	return !(this->operator bool());
}


template <typename Key, bool isConst>
inline typename SinglyLinkedListIterator<Key, isConst>::baseIteratorPtr
SinglyLinkedListIterator<Key, isConst>::clone() const
{
	return baseIteratorPtr{ new SinglyLinkedListIterator<Key, isConst>{*this} };
}


template <typename Key, bool isConst>
inline bool operator==(const SinglyLinkedListIterator<Key, isConst>& lhs, const SinglyLinkedListIterator<Key, isConst>& rhs)
{
	return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
}


template <typename Key, bool isConst>
inline bool operator!=(const SinglyLinkedListIterator<Key, isConst>& lhs, const SinglyLinkedListIterator<Key, isConst>& rhs)
{
	return !(lhs == rhs);
}