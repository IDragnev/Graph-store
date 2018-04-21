#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

template <typename Key>
struct Node
{
public:
	Node(const Key& data, Node<Key>* next = nullptr);                 

public:  
	Node<Key>* next;                                
	Key data;                                          
};


template <typename Key>
Node<Key>::Node(const Key& data, Node<Key>* next)
	:
	next(next),
	data(data)
{
}
#endif // __NODE_H_INCLUDED__