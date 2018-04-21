#ifndef __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__
#define __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__

#include "Node.h"
#include "../Iterator abstraction/Iterator.h"

template <typename Key>
class SinglyLinkedListIterator: public Iterator<Key>
{
	template <typename T>
	friend class SinglyLinkedList;
private:
	SinglyLinkedListIterator(Node<Key>* startNode, const SinglyLinkedList<Key>* owner);

public:
	virtual ~SinglyLinkedListIterator() override = default;

	virtual Key& getCurrent() override;
	virtual void goToNext() override;
	virtual bool isFinished() const override;
	virtual SinglyLinkedListIterator<Key>* clone() const override;

	template <typename Key>
	friend bool operator==(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs);
	template <typename Key>
	friend bool operator!=(const SinglyLinkedListIterator<Key>& lhs, const SinglyLinkedListIterator<Key>& rhs);

private:
	Node<Key>* current;            
	const SinglyLinkedList<Key>* owner;
};

#include "SinglyLinkedListIteratorImpl.hpp"
#endif //__SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__