#ifndef __FOR_LIST_ITERATOR_H_INCLUDED__
#define __FOR_LIST_ITERATOR_H_INCLUDED__

#include "Node.h"

template <typename Key>
class ForwardListIterator
{
	template <typename Key>
	friend class ForwardList;
private:
	ForwardListIterator(Node<Key>* startNode, const ForwardList<Key>* owner);

public:
	Key& operator*();

	ForwardListIterator<Key>& operator++();   
	ForwardListIterator<Key> operator++(int); 

	bool operator!()const;             
	operator bool()const;             
	
	template <typename Key>
	friend bool operator==(const ForwardListIterator<Key>&, const ForwardListIterator<Key>&);
	template <typename Key>
	friend bool operator!=(const ForwardListIterator<Key>&, const ForwardListIterator<Key>&);

private:
	Node<Key>* current;            
	const ForwardList<Key>* owner;
};

#include "ForwardListIteratorImpl.hpp"
#endif //__LIST_ITERATOR_H_INCLUDED__