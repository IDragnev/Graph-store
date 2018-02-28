#ifndef __SINGLY_LINKED_LIST_H_INCLUDED__
#define __SINGLY_LINKED_LIST_H_INCLUDED__

#include "Node.h"
#include "ForwardListIterator.h"
#include <utility>
#include <assert.h>
#include <stdexcept>

template <typename T>
class ForwardList
{
public:
	ForwardList();
	ForwardList(ForwardList<T>&&);
	ForwardList(const ForwardList<T>&);
	~ForwardList();

	ForwardList<T>& operator=(ForwardList<T>&&);
	ForwardList<T>& operator=(const ForwardList<T>&);

	int getCount()const;                             
	bool isEmpty()const;                              

	ForwardListIterator<T> getHead();                         
	ForwardListIterator<T> getTail();

	void appendList(ForwardList<T>&&);
	void appendList(const ForwardList<T>&);          

	void removeAll();

public:
	void setHead(const T&);                           
	void setTail(const T&);                          

	void addAsHead(const T&);                         
	void addAsTail(const T&);                         

	void removeHead();                                
	void removeTail();                          

	void removeAt(ForwardListIterator<T>& it); 
	void removeBefore(ForwardListIterator<T>& it);
	void removeAfter(ForwardListIterator<T>& it);

	void insertAfter(ForwardListIterator<T>& it, const T&);          
	void insertBefore(ForwardListIterator<T>& it, const T&);      

private:
	int count;                                         
	Node<T>* head;                                  
	Node<T>* tail;                                    

private:
	void checkIsEmtpy()const;    
	void nullMembers();

	Node<T>* findNodeBefore(const Node<T>* node)const; 

	void removeAt(Node<T>* node);
	void insertAfter(Node<T>* node, const T&);
	void insertBefore(Node<T>* node, const T&);

	void appendChain(Node<T>* first, Node<T>* last, int count);

	void swapContentsWith(ForwardList<T>);

private:
	static Node<T>* cloneChain(const Node<T>* firstNode, Node<T>** lastNode = nullptr);
	static void clearChain(const Node<T>* firstNode);  
};

#include "ForwardListImpl.hpp"
#endif //__SINGLY_LINKED_LIST_H_INCLUDED__