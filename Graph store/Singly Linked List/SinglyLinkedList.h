#ifndef __SINGLY_LINKED_LIST_H_INCLUDED__
#define __SINGLY_LINKED_LIST_H_INCLUDED__

#include "Node.h"
#include "SinglyLinkedListIterator.h"

template <typename T>
class SinglyLinkedList
{
private:
	static_assert(std::is_copy_constructible<T>::value, "SinglyLinkedList<T> requires T to be copy constructible");

	typedef unsigned unsignedInteger;

public:
	typedef SinglyLinkedListIterator<T, false> Iterator;
	typedef SinglyLinkedListIterator<T, true> ConstIterator;

	SinglyLinkedList();
	SinglyLinkedList(SinglyLinkedList<T>&& source);
	SinglyLinkedList(const SinglyLinkedList<T>& source);
	~SinglyLinkedList();

	SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& rhs);
	SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& rhs);

public:
	void insert(const T& item);
	void insertAsHead(const T& item);                         
	void insertAsTail(const T& item);                         
	void insertAfter(Iterator& iterator, const T& item);
	void insertBefore(Iterator& iterator, const T& item);

	void removeHead();                                
	void removeTail();                          
	void removeAt(Iterator& iterator);
	void removeBefore(Iterator& iterator);
	void removeAfter(Iterator& iterator);

	void appendList(SinglyLinkedList<T>&& other);
	void appendList(const SinglyLinkedList<T>& other);    

	void empty();
	bool isEmpty() const;                              
	unsignedInteger getCount() const;  

	ConstIterator getHeadConstIterator() const;
	ConstIterator getTailConstIterator() const;
	Iterator getHeadIterator();
	Iterator getTailIterator();
	
	void setHead(const T& item);                           
	void setTail(const T& item);  
	const T& getHead() const;
	const T& getTail() const;

private:
	void swapContentsWithReconstructedParameter(SinglyLinkedList<T> other);
	void copyFrom(const SinglyLinkedList<T>& source);
	void copyChainOf(const SinglyLinkedList<T>& source);
	void appendContentOf(SinglyLinkedList<T>&& source);

	void clearCurrentChain();
	void nullifyMembers();

	void removeAt(Node<T>* node);
	void insertAfter(Node<T>* node, const T& item);
	void insertBefore(Node<T>* node, const T& item);
	Node<T>* findNodeBefore(const Node<T>* node) const; 

	bool validateOwnershipOf(const Iterator& iterator) const;

private:
	unsignedInteger count;                                         
	Node<T>* head;                                  
	Node<T>* tail;                                    
};

#include "SinglyLinkedListImpl.hpp"
#endif //__SINGLY_LINKED_LIST_H_INCLUDED__