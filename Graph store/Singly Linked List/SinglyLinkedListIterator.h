#ifndef __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__
#define __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__

#include "Node.h"
#include "..\Type selector\BaseIteratorSelector.h"

template <typename T>
class SinglyLinkedList;

template <typename Key, bool isConst = false>
class SinglyLinkedListIterator : public BaseIteratorSelector<isConst, Key>::result
{
private:
	friend class SinglyLinkedListIterator<Key, true>;
	friend class SinglyLinkedList<Key>;

	typedef typename TypeSelector<isConst, const Key&, Key&>::result reference;
	typedef typename TypeSelector<isConst, const Node<Key>*, Node<Key>*>::result nodePtr;
	typedef typename BaseIteratorSelector<isConst, Key>::result baseIterator;
	typedef std::unique_ptr<baseIterator> baseIteratorPtr;

public:
	SinglyLinkedListIterator(const SinglyLinkedListIterator<Key, false>& source);
	virtual ~SinglyLinkedListIterator() override = default;

	virtual reference getCurrent() const override;
	virtual void goToNext() override;
	virtual bool isFinished() const override;
	virtual baseIteratorPtr clone() const override;

	reference operator*() const;

	SinglyLinkedListIterator<Key, isConst>& operator++();
	SinglyLinkedListIterator<Key, isConst> operator++(int);

	bool operator!() const;
	operator bool() const;

private:
	SinglyLinkedListIterator(nodePtr startNode, const SinglyLinkedList<Key>* owner);

private:
	nodePtr current;            
	const SinglyLinkedList<Key>* owner;
};

#include "SinglyLinkedListIteratorImpl.hpp"
#endif //__SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__