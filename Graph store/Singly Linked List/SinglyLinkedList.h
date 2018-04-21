#ifndef __SINGLY_LINKED_LIST_H_INCLUDED__
#define __SINGLY_LINKED_LIST_H_INCLUDED__

#include "Node.h"
#include "SinglyLinkedListIterator.h"
#include <utility>
#include <assert.h>
#include <stdexcept>

template <typename T>
class SinglyLinkedList
{
private:
	typedef unsigned countType;

public:
	SinglyLinkedList();
	SinglyLinkedList(SinglyLinkedList<T>&& source);
	SinglyLinkedList(const SinglyLinkedList<T>& source);
	~SinglyLinkedList();

	SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& rhs);
	SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& rhs);

	countType getCount() const;                             
	bool isEmpty() const;                              
	void empty();

	SinglyLinkedListIterator<T> getHeadIterator();                         
	SinglyLinkedListIterator<T> getTailIterator();

	void appendList(SinglyLinkedList<T>&& other);
	void appendList(const SinglyLinkedList<T>& other);          

public:
	void setHead(const T& item);                           
	void setTail(const T& item);                          

	void insert(const T& item);
	void insertAsHead(const T& item);                         
	void insertAsTail(const T& item);                         

	void removeHead();                                
	void removeTail();                          

	void removeAt(SinglyLinkedListIterator<T>& iterator); 
	void removeBefore(SinglyLinkedListIterator<T>& iterator);
	void removeAfter(SinglyLinkedListIterator<T>& iterator);

	void insertAfter(SinglyLinkedListIterator<T>& iterator, const T& item);
	void insertBefore(SinglyLinkedListIterator<T>& iterator, const T& item);

private:
	void throwExceptionIfEmpty() const; 
	void throwExceptionIfInvalid(const SinglyLinkedListIterator<T>& iterator) const;

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