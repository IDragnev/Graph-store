#ifndef __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__
#define __SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__

#include "..\Type selector\BaseIteratorSelector.h"

template <typename T>
class SinglyLinkedList;
template <typename T>
struct Node;


template <typename Key, bool isConst = false>
class SinglyLinkedListIterator : public BaseIteratorSelector<isConst, Key>::result
{
private:
	friend class SinglyLinkedListIterator<Key, true>;
	friend class SinglyLinkedList<Key>;

	using nodePtr = typename TypeSelector<isConst, const Node<Key>*, Node<Key>*>::result;
	using baseIterator = typename BaseIteratorSelector<isConst, Key>::result;
	using baseIteratorPtr = std::unique_ptr<baseIterator>;

public:
	using value_type = Key;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;
	using reference = typename TypeSelector<isConst, const Key&, Key&>::result;
	using pointer = typename TypeSelector<isConst, const Key*, Key*>::result;

public:
	SinglyLinkedListIterator(const SinglyLinkedListIterator<Key, false>& source);
	virtual ~SinglyLinkedListIterator() override = default;

	virtual reference getCurrent() const override;
	virtual void goToNext() override;
	virtual bool isFinished() const override;
	virtual baseIteratorPtr clone() const override;

	reference operator*() const;
	pointer operator->() const;

	SinglyLinkedListIterator<Key, isConst>& operator++();
	SinglyLinkedListIterator<Key, isConst> operator++(int);

	template <typename Key, bool isConst>
	friend bool operator==(const SinglyLinkedListIterator<Key, isConst>& lhs, const SinglyLinkedListIterator<Key, isConst>& rhs);

private:
	SinglyLinkedListIterator(nodePtr startNode, const SinglyLinkedList<Key>* owner);

private:
	nodePtr current;            
	const SinglyLinkedList<Key>* owner;
};

template <typename Key, bool isConst>
bool operator!=(const SinglyLinkedListIterator<Key, isConst>& lhs, const SinglyLinkedListIterator<Key, isConst>& rhs);

#include "SinglyLinkedListIteratorImpl.hpp"
#endif //__SINGLY_LINKED_LIST_ITERATOR_H_INCLUDED__