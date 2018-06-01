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
	static_assert(std::is_copy_constructible<T>::value, "SinglyLinkedList<T> requires T to be copy constructible");

	typedef unsigned countType;

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
	void swapContentsWithReconstructedParameter(SinglyLinkedList<T> other);

	void removeAt(Node<T>* node);
	void insertAfter(Node<T>* node, const T& item);
	void insertBefore(Node<T>* node, const T& item);

	void appendChainAndUpdateCount(Node<T>* first, Node<T>* last, countType count);

	void nullifyMembers();
	
	bool validateOwnershipOf(const Iterator& iterator) const;

	Node<T>* findNodeBefore(const Node<T>* node) const; 

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