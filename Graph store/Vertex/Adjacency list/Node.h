#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <utility>

template <typename Key>
struct Node
{
public:
	Node(const Key& data, Node<Key>* next = nullptr);
	Node(Key&& data, Node<Key>* next = nullptr);

	bool hasSuccessor()const;                     

public:  
	Node<Key>* next;                                
	Key data;                                          
};

#include "NodeImpl.hpp"
#endif // __NODE_H_INCLUDED__