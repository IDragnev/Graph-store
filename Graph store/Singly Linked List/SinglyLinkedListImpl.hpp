#include <utility>
#include <assert.h>


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() :
	count(0),
	head(nullptr),
	tail(nullptr)
{
}


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& source) :
	SinglyLinkedList<T>()
{
	if (!source.isEmpty())
	{
		copyFrom(source);
	}
}


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& source) :
	head(source.head),
	tail(source.tail),
	count(source.count)
{
	source.nullifyMembers();
}


template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& rhs)
{
	if (this != &rhs)
	{
		swapContentsWithReconstructedParameter(rhs);
	}

	return *this;
}


template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList<T>&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWithReconstructedParameter(std::move(rhs));
	}

	return *this;
}


template <typename T>
inline SinglyLinkedList<T>::~SinglyLinkedList()
{
	clearCurrentChain();
}


template <typename T>
void SinglyLinkedList<T>::appendList(const SinglyLinkedList<T>& source)
{
	SinglyLinkedList<T> temporary(source);
	appendList(std::move(temporary));
}


template <typename T>
void SinglyLinkedList<T>::appendList(SinglyLinkedList<T>&& source)
{
	if (!source.isEmpty())
	{
		appendContentOf(std::move(source));
		source.nullifyMembers();
	}
}


template <typename T>
inline void SinglyLinkedList<T>::empty()
{
	clearCurrentChain();
	nullifyMembers();
} 


template <typename T>
inline void SinglyLinkedList<T>::removeAt(Iterator& iterator)
{
	assert(validateOwnershipOf(iterator));

	removeAt(iterator.current);
	iterator.current = nullptr;
}


template <typename T>
inline void SinglyLinkedList<T>::removeAfter(Iterator& iterator)
{
	assert(validateOwnershipOf(iterator));

	removeAt(iterator.current->next);
}


template <typename T>
inline void SinglyLinkedList<T>::removeBefore(Iterator& iterator)
{
	assert(validateOwnershipOf(iterator));

	removeAt(findNodeBefore(iterator.current));
}


template <typename T>
inline void SinglyLinkedList<T>::insertAfter(Iterator& iterator, const T& item)
{
	assert(validateOwnershipOf(iterator));

	insertAfter(iterator.current, item);
}


template <typename T>
inline void SinglyLinkedList<T>::insertBefore(Iterator& iterator, const T& item)
{
	assert(validateOwnershipOf(iterator));

	insertBefore(iterator.current, item);
}


template <typename T>
inline void SinglyLinkedList<T>::insert(const T& item)
{
	insertAsTail(item);
}


template <typename T>
void SinglyLinkedList<T>::insertAsTail(const T& item)
{
	Node<T>* newTail = new Node<T>(item);

	if (isEmpty())
	{
		head = newTail;
	}
	else 
	{	
		tail->next = newTail;
	}

	tail = newTail;
	++count;
}


template <typename T>
void SinglyLinkedList<T>::insertAsHead(const T& item)
{
	Node<T>* newHead = new Node<T>(item, head);

	if (isEmpty())
	{
		tail = newHead;
	}

	head = newHead;
	++count;
}


template <typename T>
void SinglyLinkedList<T>::removeHead()
{
	assert(!isEmpty());

	Node<T>* oldHead = head;	
	head = head->next;

	if (!head)
	{
		tail = nullptr;
	}

	delete oldHead;
	--count;
}


template <typename T>
inline void SinglyLinkedList<T>::removeTail()
{
	removeAt(tail);
}


template <typename T>
void SinglyLinkedList<T>::removeAt(Node<T>* nodeToRemove)
{
	if (!nodeToRemove)
	{
		return;
	}
	else if (nodeToRemove == head)
	{
		removeHead();
	}
	else
	{
		Node<T>* nodeBefore = findNodeBefore(nodeToRemove);
		assert(nodeBefore);

		if (nodeToRemove == tail)
		{
			tail = nodeBefore;
		}

		nodeBefore->next = nodeToRemove->next;
		delete nodeToRemove;
		--count;
	}
}


template <typename T>
void SinglyLinkedList<T>::insertAfter(Node<T>* nodeToInsertAfter, const T& item)
{
	if (!nodeToInsertAfter || nodeToInsertAfter == tail)
	{
		insertAsTail(item);
	}
	else 
	{
		assert(nodeToInsertAfter->next);

		Node<T>* newNode = new Node<T>(item, nodeToInsertAfter->next);
		nodeToInsertAfter->next = newNode;
		++count;
	}
}


template <typename T>
void SinglyLinkedList<T>::insertBefore(Node<T>* nodeToInsertBefore, const T& item)
{
	if (!nodeToInsertBefore || nodeToInsertBefore == head)
	{
		insertAsHead(item);
	}
	else
	{
		Node<T>* previous = findNodeBefore(nodeToInsertBefore);
		assert(previous);
		insertAfter(previous, item);
	}
}


template <typename T>
void SinglyLinkedList<T>::copyFrom(const SinglyLinkedList<T>& source)
{
	assert(isEmpty());
	assert(!source.isEmpty());
	try
	{
		copyChainOf(source);
		count = source.count;
	}
	catch (...)
	{
		empty();
		throw;
	}
}


template <typename T>
void SinglyLinkedList<T>::copyChainOf(const SinglyLinkedList<T>& source)
{
	assert(!head);
	assert(source.head);

	head = new Node<T>(source.head->data);
	const Node<T>* nodeToCopy = source.head->next;
	tail = head;

	while (nodeToCopy)
	{
		tail->next = new Node<T>(nodeToCopy->data);
		tail = tail->next;
		nodeToCopy = nodeToCopy->next;
	}
}


template <typename T>
void SinglyLinkedList<T>::appendContentOf(SinglyLinkedList<T>&& source)
{
	assert(source.head);
	assert(source.tail);

	if (isEmpty())
	{
		head = source.head;
	}
	else
	{
		tail->next = source.head;
	}

	tail = source.tail;
	count += source.count;
}


template <typename T>
inline void SinglyLinkedList<T>::swapContentsWithReconstructedParameter(SinglyLinkedList<T> temp)
{
	std::swap(this->head, temp.head);
	std::swap(this->tail, temp.tail);
	std::swap(this->count, temp.count);
}


template <typename T>
inline void SinglyLinkedList<T>::nullifyMembers()
{
	count = 0;
	head = nullptr;
	tail = nullptr;
}


template <typename T>
inline void SinglyLinkedList<T>::clearCurrentChain()
{
	while (head)
	{
		Node<T>* oldHead = head;
		head = head->next;
		delete oldHead;
	}
}


template <typename T>
Node<T>* SinglyLinkedList<T>::findNodeBefore(const Node<T>* node) const
{
	assert(node);

	Node<T>* current = head;

	while (current && current->next != node)
	{
		current = current->next;
	}

	return current;
}


template <typename T>
inline bool SinglyLinkedList<T>::validateOwnershipOf(const Iterator& iterator) const
{
	return iterator.owner == this;
}


template <typename T>
inline const T& SinglyLinkedList<T>::getHead() const
{
	assert(!isEmpty());

	return head->data;
}


template <typename T>
inline const T& SinglyLinkedList<T>::getTail() const
{
	assert(!isEmpty());

	return tail->data;
}


template <typename T>
inline void SinglyLinkedList<T>::setHead(const T& data)
{
	assert(!isEmpty());

	head->data = data;
}


template <typename T>
inline void SinglyLinkedList<T>::setTail(const T& data)
{
	assert(!isEmpty());

	tail->data = data;
}


template <typename T>
inline bool SinglyLinkedList<T>::isEmpty() const
{
	return count == 0;
}


template <typename T>
inline typename SinglyLinkedList<T>::unsignedInteger SinglyLinkedList<T>::getCount() const
{
	return count;
}


template <typename T>
inline typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::getHeadConstIterator() const
{
	return ConstIterator(head, this);
}


template <typename T>
inline typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::getTailConstIterator() const
{
	return ConstIterator(tail, this);
}


template <typename T>
inline typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::getHeadIterator()
{
	return Iterator(head, this);
}


template <typename T>
inline typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::getTailIterator()
{
	return Iterator(tail, this);
}