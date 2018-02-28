
//---------------------------------------------------------------------------------------
//
//GETTERS 
//


template <typename T>
inline bool ForwardList<T>::isEmpty()const
{
	return count == 0;
}


//
//if the list is empty, an exception is thrown
//
//it will be called in each operation that cannot be
//executed if the list is emtpy
//
template <typename T>
inline void ForwardList<T>::checkIsEmtpy()const
{
	if (isEmpty())
		throw std::logic_error("List is empty!");
}


template <typename T>
inline int ForwardList<T>::getCount()const
{
	return count;
}


template <typename T>
ForwardListIterator<T> ForwardList<T>::getHead()
{
	return ForwardListIterator<T>(head, this);
}


template <typename T>
ForwardListIterator<T> ForwardList<T>::getTail()
{
	return ForwardListIterator<T>(tail, this);
}



//-------------------------------------------------------------------------------------------
//
//SET
//


template <typename T>
void ForwardList<T>::nullMembers()
{
	head = nullptr;
	tail = nullptr;
	count = 0;
}


//
//sets the data of the head node with the passed value
//
// \ if the list is empty, an exception is thrown
//
template <typename T>
void ForwardList<T>::setHead(const T& value)
{
	checkIsEmtpy();

	head->data = value;
}



//
//sets the data of the tail node with the passed value
//
// \ if the list is empty, an exception is thrown
//
template <typename T>
void ForwardList<T>::setTail(const T& value)
{
	checkIsEmtpy();

	tail->data = value;
}



//-------------------------------------------------------------------------------------------
//
//UTILITY
//


//
//free each node in the chain
//
// \ if the sent address is nullptr the function does nothing
//
template <typename T>
void ForwardList<T>::clearChain(const Node<T>* firstNode)
{
	if (firstNode)
	{
		const Node<T>* current = firstNode;
		const Node<T>* oldNode = firstNode;

		//while there is an actual node to delete
		while (current)
		{
			//save current's address
			oldNode = current;

			//move forward in the chain
			current = current->next;

			//free old
			delete oldNode;
		}
	}
}



//
//free the chain of nodes
//and null the members
//
template <typename T>
void ForwardList<T>::removeAll()
{
	clearChain(head);

	nullMembers();
}




//
//clone a chain by the address of its first node
//
//the function assumes firstNode is a valid pointer
//
// \ if endOfChain points to a valid pointer
//   it is set to point to the address of the
//   last node in the cloned chain
//
template <typename T>
Node<T>* ForwardList<T>::cloneChain(const Node<T>* firstNode, Node<T>** endOfChain)
{
	Node<T>* newChain = nullptr;
	assert(firstNode);

	try
	{
		//the start of the new chain
		newChain = new Node<T>(firstNode->data);

		//pointer to the node we will copy next (initially points to the second node of the passed chain)
		const Node<T>* readFrom = firstNode->next;

		//pointer to the node to which we will connect the next node
		Node<T>* connectTo = newChain;

		//while there is an actual node to copy
		while (readFrom)
		{
			//append to chain
			connectTo->next = new Node<T>(readFrom->data);

			//move to current end of chain
			connectTo = connectTo->next;

			//move to the next node to be copied
			readFrom = readFrom->next;
		}

		if (endOfChain)
			*endOfChain = connectTo;

		return newChain;
	} 
	catch (std::bad_alloc& ex)
	{		
		clearChain(newChain);

		throw;
	}
}


//
//append other list's chain of nodes to this
//
template <typename T>
void ForwardList<T>::appendList(const ForwardList<T>& other)
{
	//if other is empty, leave
	if (other.isEmpty())
		return;

	//a pointer to save the end of the new chain
	Node<T>* endOfChain = nullptr;

	//clone other list's chain of nodes
	Node<T>* newChain = cloneChain(other.head, &endOfChain);

	//append the cloned chain to the current one
	appendChain(newChain, endOfChain, other.count);
}



//
//append source's chain of nodes to this directly, 
//leaving source in a valid empty state
//
template <typename T>
void ForwardList<T>::appendList(ForwardList<T>&& source)
{
	//if source is empty, leave
	if (source.isEmpty())
		return;

	//append source's chain
	appendChain(source.head, source.tail, source.count);

	//and null it
	source.nullMembers();
}




//
//append the sent chain to the current chain and update count
//
template <typename T>
void ForwardList<T>::appendChain(Node<T>* first, Node<T>* last, int count)
{
	assert(first && last);

	//if the list is empty, attach the chain to the head pointer
	if (isEmpty())
	{
		this->head = first;
	}
	else //attach it to the tail node
	{
		this->tail->next = first;
	}

	//update tail and count
	this->tail = last;
	this->count += count;
}



//
//returns the node before the passed one 
//
// \ if no matching node is found, nullptr is returned
//
// \ if the list is empty, nullptr is returned
//
template <typename T>
Node<T>* ForwardList<T>::findNodeBefore(const Node<T>* node)const
{
	Node<T>* current = head;

	//while there is an actual node and its successor is not the one 
	//we are searching for, move forward in the list
	while (current && current->next != node)
	{
		current = current->next;
	}

	return current;
}





//------------------------------------------------------------------------------------
//
//CTORS
//


//
//the list is empty when default-constructed
//
template <typename T>
ForwardList<T>::ForwardList()
	:
	count(0),
	head(nullptr),
	tail(nullptr)
{
	;
}


//
//initialize members as 0
//and append other's chain of nodes (copied) 
//
template <typename T>
ForwardList<T>::ForwardList(const ForwardList<T>& other)
	:
	count(0),
	head(nullptr),
	tail(nullptr)
{
	appendList(other);
	assert(count == other.count);
}



//
//copy assignment
//
//copy-construct 'swapContentsWith' parameter from 'other'
//and let it do the work
//
template <typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& other)
{
	if (this != &other)
	{
		//parameter is copy-constructed from other
		swapContentsWith(other);
	}

	return *this;
}


//
//free the chain of nodes
//
template <typename T>
ForwardList<T>::~ForwardList()
{
	clearChain(head);
}



//
//move constructor
//
//'steal' source's data and then null source 
//(source is left in a valid empty state)
//
template <typename T>
ForwardList<T>::ForwardList(ForwardList<T>&& source)
	:
	head(source.head),
	tail(source.tail),
	count(source.count)
{
	source.nullMembers();
}



//
//move assignment
//
//move source in the 'swapContentsWith' parameter (which is a temporary object)
//and let it do the work
//
template <typename T>
ForwardList<T>& ForwardList<T>::operator=(ForwardList<T>&& source)
{
	if (this != &source)
	{
		//source is moved into parameter
		swapContentsWith(std::move(source));
	}

	return *this;
}




//
// construct temp (parameter) with the passed argument,
// swap contents with this and let temp destroy old data
//
// \ if the argument is an rvalue, temp will be move-constructed from it
// 
// \ if the argument is an lvalue, temp will be copy-constructed from it
//
template <typename T>
void ForwardList<T>::swapContentsWith(ForwardList<T> temp)
{
	std::swap(this->head, temp.head);
	std::swap(this->tail, temp.tail);
	std::swap(this->count, temp.count);

	//temp dies here...
}



//-------------------------------------------------------------------------------------
//
//INSERTION AND REMOVAL
//


//
//add a node with the passed data as a head node
//
template <typename T>
void ForwardList<T>::addAsHead(const T& value)
{
	//the new node's successor is the current head node
	Node<T>* newHead = new Node<T>(value, this->head);

	//if the list is emtpy, set tail
	if (isEmpty())
	{
		this->tail = newHead;
	}

	//update head and count
	this->head = newHead;

	++count;
}



//
//add a node with the passed data as the new tail
//
template <typename T>
void ForwardList<T>::addAsTail(const T& value)
{
	//has no successor as a tail
	Node<T>* newTail = new Node<T>(value);

	//if the list is empty, update head
	if (isEmpty())
	{
		this->head = newTail;
	}
	else //update the successor of current tail
	{	
		this->tail->next = newTail;
	}

	//update tail and count
	this->tail = newTail;

	++count;
}




//
//remove the head node
//
// \ if the list is empty, an exception is thrown
//
template <typename T>
void ForwardList<T>::removeHead()
{
	checkIsEmtpy();

	assert(head);

	//keep the address of the old head node
	Node<T>* oldHead = head;

	//move head to the next node in the list
	head = head->next;

	//if this was the only node in the list
	//update tail to nullptr 
	if(head == nullptr)
	{
		tail = nullptr;
	}

	//free old head and update count
	delete oldHead;

	--count;
}




//
//remove the specified node from the list
//
// \ if the pointer is nullptr, the function does nothing
//
template <typename T>
void ForwardList<T>::removeAt(Node<T>* node)
{
	if (node)
	{
		if (node == head)
		{
			removeHead();
		}
		//if it is not the head node, there are at least
		//two nodes in the list
		else
		{
			//find the node before the one we want to delete
			Node<T>* nodeBefore = findNodeBefore(node);

			assert(nodeBefore);

			//if the node we are about to delete is the tail, update tail
			if (node == tail)
			{
				tail = nodeBefore;
			}

			//update the successor of the previous node
			nodeBefore->next = node->next;

			//free node and update count
			delete node;

			--count;
		}
	}
}


//
//remove the node the sent iterator is pointing to
//
// \ if the iterator is not from this list an exception is thrown
//
template <typename T>
void ForwardList<T>::removeAt(ForwardListIterator<T>& iterator)
{
	if (iterator.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	removeAt(iterator.current);

	//invalidate iterator
	iterator.current = nullptr;
}



template <typename T>
void ForwardList<T>::removeTail()
{
	return removeAt(tail);
}



//
//insert a node with the sent data
//exactly after the sent node
//
// \ if the pointer is nullptr
//   or it points to the tail node
//   addAsTail is called 
//
template <typename T>
void ForwardList<T>::insertAfter(Node<T>* node, const T& data)
{
	if (!node || node == tail)
	{
		addAsTail(data);
	}
	else //insert after it
	{
		//if it does not point to the tail (and is not nullptr), it has a successor
		assert(node->next);

		//newNode's successor is node's current successor
		Node<T>* newNode = new Node<T>(data, node->next);

		//update node's successor as the new node
		node->next = newNode;

		++count;
	}
}




//
//add a new node with the sent data
//exactly after the sent iterator's current
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::insertAfter(ForwardListIterator<T>& iterator, const T& data)
{
	if (iterator.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	insertAfter(iterator.current, data);
}



//
//insert a node with the sent data 
//exactly before the sent node
//
// \ if the pointer is nullptr or points to the head,
//    addAsHead is called
//
template <typename T>
void ForwardList<T>::insertBefore(Node<T>* node, const T& data)
{
	if (!node || node == head)
	{
		addAsHead(data);
	}
	else
	{
		//it has a predecessor, because it is not the head node (and is not null)
		Node<T>* previous = findNodeBefore(node);
		
		assert(previous);

		//insert after its predecessor
		insertAfter(previous, data);
	}
}



//
//insert a new node with the sent data
//exactly before the sent iterator's current
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::insertBefore(ForwardListIterator<T>& iterator, const T& data)
{
	if (iterator.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	insertBefore(iterator.current, data);
}



//
//remove the node after the one the iterator points to
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::removeAfter(ForwardListIterator<T>& iterator)
{
	if (iterator.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	removeAt(iterator.current->next);
}




//
//remove the node before the one the iterator points to
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::removeBefore(ForwardListIterator<T>& iterator)
{
	if (iterator.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	//find the one before it and remove it
	remoteAt( findNodeBefore(iterator.current) );
}