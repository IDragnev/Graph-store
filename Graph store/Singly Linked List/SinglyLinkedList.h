#ifndef __SINGLY_LINKED_LIST_H_INCLUDED__
#define __SINGLY_LINKED_LIST_H_INCLUDED__

#include <utility>
#include <assert.h>
#include <stdexcept>
#include "Node.h"
#include "SinglyLinkedListIterator.h"

template <typename T>
class SinglyLinkedList
{
private:
	typedef unsigned countType;
	//TODO : insert static asserts for copy_constructable and copy_assignable

public:
	typedef SinglyLinkedListIterator<T, false> Iterator;
	typedef SinglyLinkedListIterator<T, true> ConstIterator;

	SinglyLinkedList();
	SinglyLinkedList(SinglyLinkedList<T>&& source);
	SinglyLinkedList(const SinglyLinkedList<T>& source);
	~SinglyLinkedList();

	SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& rhs);
	SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& rhs);

	countType getCount() const;                             
	bool isEmpty() const;                              
	void empty();

	ConstIterator getHeadConstIterator() const;
	ConstIterator getTailConstIterator() const;
	Iterator getHeadIterator();
	Iterator getTailIterator();

	void appendList(SinglyLinkedList<T>&& other);
	void appendList(const SinglyLinkedList<T>& other);          

public:
	const T& getHead() const;
	const T& getTail() const;

	void setHead(const T& item);                           
	void setTail(const T& item);                          

	void insert(const T& item);
	void insertAsHead(const T& item);                         
	void insertAsTail(const T& item);                         

	void removeHead();                                
	void removeTail();                          

	void removeAt(Iterator& iterator);
	void removeBefore(Iterator& iterator);
	void removeAfter(Iterator& iterator);

	void insertAfter(Iterator& iterator, const T& item);
	void insertBefore(Iterator& iterator, const T& item);

private:
	void throwExceptionIfEmpty() const; 
	void throwExceptionIfInvalid(const Iterator& iterator) const;

	void nullifyMembers();

	Node<T>* findNodeBefore(const Node<T>* node) const; 

	void removeAt(Node<T>* node);
	void insertAfter(Node<T>* node, const T& item);
	void insertBefore(Node<T>* node, const T& item);

	void appendChainAndUpdateCount(Node<T>* first, Node<T>* last, countType count);

	void swapContentsWithReconstructedParameter(SinglyLinkedList<T> other);

private:
	static Node<T>* cloneChainStartingAt(const Node<T>* firstNode, Node<T>** lastNode = nullptr);
	static void clearChainStartingAt(const Node<T>* firstNode);  

private:
	countType count;                                         
	Node<T>* head;                                  
	Node<T>* tail;                                    
};

#include "SinglyLinkedListImpl.hpp"
#endif //__SINGLY_LINKED_LIST_H_INCLUDED__