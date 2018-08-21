

template <typename T>
template <typename Item>
SinglyLinkedList<T>::Node<Item>::Node(const Item& data, Node<Item>* next) :
	next{ next },
	data{ data }
{
}


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() :
	count{ 0 }, 
	head{ nullptr },
	tail{ nullptr }
{
}


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(std::initializer_list<T> source) :
	SinglyLinkedList()
{
	std::for_each(source.begin(), source.end(), [&](const T& item) { insert(item); });
}


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& source) :
	head{ source.head },
	tail{ source.tail },
	count{ source.count }
{
	source.nullifyMembers();
}


template <typename T>
inline void SinglyLinkedList<T>::nullifyMembers()
{
	count = 0;
	head = tail = nullptr;
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
inline void SinglyLinkedList<T>::swapContentsWithReconstructedParameter(SinglyLinkedList<T> temp)
{
	std::swap(head, temp.head);
	std::swap(tail, temp.tail);
	std::swap(count, temp.count);
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
inline bool SinglyLinkedList<T>::isEmpty() const
{
	return count == 0;
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

	head = new Node<T>{ source.head->data };
	const Node<T>* nodeToCopy = source.head->next;
	tail = head;

	while (nodeToCopy)
	{
		tail->next = new Node<T>{ nodeToCopy->data };
		tail = tail->next;
		nodeToCopy = nodeToCopy->next;
	}
}


template <typename T>
void SinglyLinkedList<T>::empty()
{
	clearCurrentChain();
	nullifyMembers();
}


template <typename T>
inline SinglyLinkedList<T>::~SinglyLinkedList()
{
	clearCurrentChain();
}


template <typename T>
void SinglyLinkedList<T>::clearCurrentChain()
{
	while (head)
	{
		Node<T>* oldHead = head;
		head = head->next;
		delete oldHead;
	}
}


template <typename T>
inline void SinglyLinkedList<T>::appendList(const SinglyLinkedList<T>& source)
{
	SinglyLinkedList<T> temporary{ source };
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
inline void SinglyLinkedList<T>::insert(const T& item)
{
	insertAsTail(item);
}


template <typename T>
void SinglyLinkedList<T>::insertAsTail(const T& item)
{
	Node<T>* newTail = new Node<T>{ item };

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
	Node<T>* newHead = new Node<T>{ item, head };

	if (isEmpty())
	{
		tail = newHead;
	}

	head = newHead;
	++count;
}


template <typename T>
void SinglyLinkedList<T>::insertAfter(Node<T>* node, const T& item)
{
	if (!node || node == tail)
	{
		insertAsTail(item);
	}
	else 
	{
		assert(node->next);

		Node<T>* newNode = new Node<T>{ item, node->next };
		node->next = newNode;
		++count;
	}
}


template <typename T>
void SinglyLinkedList<T>::insertBefore(Node<T>* node, const T& item)
{
	if (!node || node == head)
	{
		insertAsHead(item);
	}
	else
	{
		Node<T>* previous = findNodeBefore(node);
		assert(previous);
		insertAfter(previous, item);
	}
}


template <typename T>
typename SinglyLinkedList<T>::Node<T>* 
SinglyLinkedList<T>::findNodeBefore(const Node<T>* node) const
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
void SinglyLinkedList<T>::removeAt(Node<T>* node)
{
	if (!node)
	{
		return;
	}
	else if (node == head)
	{
		removeHead();
	}
	else
	{
		Node<T>* nodeBefore = findNodeBefore(node);
		assert(nodeBefore);

		if (node == tail)
		{
			tail = nodeBefore;
		}

		nodeBefore->next = node->next;
		delete node;
		--count;
	}
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
inline typename SinglyLinkedList<T>::unsignedInteger SinglyLinkedList<T>::getCount() const
{
	return count;
}


template <typename T>
inline typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::getBeginConstIterator() const
{
	return ConstIterator{ head, this };
}


template <typename T>
inline typename SinglyLinkedList<T>::ConstIterator SinglyLinkedList<T>::getEndConstIterator() const
{
	return ConstIterator{ nullptr, this };
}


template <typename T>
inline typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::getBeginIterator()
{
	return Iterator{ head, this };
}


template <typename T>
inline typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::getEndIterator()
{
	return Iterator{ nullptr, this };
}


template <typename T>
inline bool operator==(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs)
{
	return std::equal(lhs.getBeginConstIterator(), lhs.getEndConstIterator(),
		              rhs.getBeginConstIterator(), rhs.getEndConstIterator());
}


template <typename T>
inline bool operator!=(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs)
{
	return !(lhs == rhs);
}