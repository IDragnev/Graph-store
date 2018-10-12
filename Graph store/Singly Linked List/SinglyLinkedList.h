#ifndef __SINGLY_LINKED_LIST_H_INCLUDED__
#define __SINGLY_LINKED_LIST_H_INCLUDED__

#include <utility>
#include <assert.h>
#include <iterator>

template <typename T>
class SinglyLinkedList
{
private:
	static_assert(std::is_copy_constructible<T>::value, 
				  "SinglyLinkedList<T> requires T to be copy constructible");

	template <typename Item>
	struct Node
	{
		Node(const Item& data, Node<Item>* next = nullptr);
		Node(Item&& data, Node<Item>* next = nullptr);

		Node<Item>* next;
		Item data;
	};

	template <typename Item, bool isConst = false>
	class SinglyLinkedListIterator
	{
	private:
		friend class SinglyLinkedList<Item>;
		using ownerPtr = const SinglyLinkedList<Item>*;
		using nodePtr = std::conditional_t<isConst, const Node<Item>*, Node<Item>*>;

	public:
		using value_type = Item;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		using reference = std::conditional_t<isConst, const Item&, Item&>;
		using pointer = std::conditional_t<isConst, const Item*, Item*>;

	public:
		SinglyLinkedListIterator(const SinglyLinkedListIterator<Item, false>& source);

		pointer operator->() const;
		reference operator*() const;

		SinglyLinkedListIterator<Item, isConst>& operator++();
		SinglyLinkedListIterator<Item, isConst> operator++(int);

		operator bool() const;
		bool operator!() const;

		template <typename Item, bool isConst>
		friend bool operator==(typename const SinglyLinkedList<Item>::SinglyLinkedListIterator<Item, isConst>& lhs, 
			                   typename const SinglyLinkedList<Item>::SinglyLinkedListIterator<Item, isConst>& rhs);

	private:
		SinglyLinkedListIterator(nodePtr startNode, ownerPtr owner);

	private:
		nodePtr current;
		ownerPtr owner;
	};

public:
	using iterator = SinglyLinkedListIterator<T, false>;
	using const_iterator = SinglyLinkedListIterator<T, true>;

	SinglyLinkedList();
	template <typename InputIt>
	SinglyLinkedList(InputIt first, InputIt last);
	SinglyLinkedList(std::initializer_list<T> source);
	SinglyLinkedList(SinglyLinkedList<T>&& source);
	SinglyLinkedList(const SinglyLinkedList<T>& source);
	~SinglyLinkedList();

	SinglyLinkedList<T>& operator=(SinglyLinkedList<T>&& rhs);
	SinglyLinkedList<T>& operator=(const SinglyLinkedList<T>& rhs);

public:
	void insert(const T& item);
	void insertAsHead(const T& item);                         
	void insertAsTail(const T& item);                         
	void insertAfter(iterator& position, const T& item);
	void insertBefore(iterator& position, const T& item);

	void removeHead();                                
	void removeTail();                          
	void removeAt(iterator& position);
	void removeBefore(iterator& position);
	void removeAfter(iterator& position);

	void appendList(SinglyLinkedList<T>&& other);
	void appendList(const SinglyLinkedList<T>& other);    

	void empty();
	bool isEmpty() const;                              
	unsigned getCount() const;  

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	
	void setHead(const T& item);                           
	void setTail(const T& item);  
	const T& getHead() const;
	const T& getTail() const;

private:
	void swapContentsWithReconstructedParameter(SinglyLinkedList<T> other);
	void appendContentOf(SinglyLinkedList<T>&& source);

	template <typename InputIt>
	void tryToBuildChain(InputIt first, InputIt last);
	template <typename InputIt>
	void buildChain(InputIt first, InputIt last);

	void clearCurrentChain();
	void nullifyMembers();

	void removeAt(Node<T>* node);
	void insertAfter(Node<T>* node, const T& item);
	void insertBefore(Node<T>* node, const T& item);
	Node<T>* findNodeBefore(const Node<T>* node) const; 

	bool validateOwnershipOf(const iterator& it) const;

private:
	unsigned count;                                         
	Node<T>* head;                                  
	Node<T>* tail;                                    
};

template <typename T>
bool operator==(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs);
template <typename T>
bool operator!=(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs);

template <typename T, bool isConst>
bool operator!=(typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& lhs,
	            typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& rhs);

#include "SinglyLinkedListImpl.hpp"
#include "SinglyLinkedListIteratorImpl.hpp"
#endif //__SINGLY_LINKED_LIST_H_INCLUDED__