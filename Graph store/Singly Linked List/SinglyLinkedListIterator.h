#ifndef __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__
#define __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__

#include "Node.h"
#include "../Iterator abstraction/Iterator.h"

template <typename Key>
class SinglyLinkedListIterator: public Iterator<Key>
{
	template <typename T>
	friend class SinglyLinkedList;
public:
	virtual ~SinglyLinkedListIterator() override = default;

	virtual Key& getCurrent() const override;
	virtual void goToNext() override;
	virtual bool isFinished() const override;
	virtual Iterator<Key>* clone() const override;

	Key& operator*() const;

	SinglyLinkedListIterator<Key>& operator++();
	SinglyLinkedListIterator<Key> operator++(int);

	bool operator!() const;
	operator bool() const;

	template <typename Key>
	friend bool operator==(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs);
	template <typename Key>
	friend bool operator!=(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs);

private:
	SinglyLinkedListIterator(Node<Key>* startNode, const SinglyLinkedList<Key>* owner);

private:
	Node<Key>* current;            
	const SinglyLinkedList<Key>* owner;
};

#include "SinglyLinkedListIteratorImpl.hpp"
#endif //__SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__