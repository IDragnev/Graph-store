
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


//
//sets the value of the head node with the passed value
//
//if the list is empty, an exception is thrown
//
template <typename T>
void ForwardList<T>::setHead(const T& value)
{
	checkIsEmtpy();

	head->data = value;
}


//
//sets the value of the tail node with the passed value
//
//if the list is empty, an exception is thrown
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
//move forward in the chain while possible
//and return the address of the last node
//
template <typename T>
Node<T>* ForwardList<T>::findEndOfChain(Node<T>* firstNode)
{
	assert(firstNode);

	Node<T>* current = firstNode;

	//while there is an actual successor
	//move forward in the chain
	while (current->next)
		current = current->next;

	return current;
}


//
//free each node in the chain
//
//if the sent address is NULL the function does nothing
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
//free the chain
//and set members back to 0
//
template <typename T>
void ForwardList<T>::removeAll()
{
	clearChain(head);

	head = NULL;
	tail = NULL;
	count = 0;
}




//
//clone the chain by the address of its first node
//
// \ if endOfChain points to a valid pointer
//   it is set to point to the address of the
//   last node in the cloned chain
//
template <typename T>
Node<T>* ForwardList<T>::cloneChain(const Node<T>* firstNode, Node<T>** endOfChain)
{
	Node<T>* newChain = NULL;
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
	} //in case of bad allocation
	catch (std::bad_alloc& ex)
	{		
		clearChain(newChain);

		throw;
	}
}


//
//append other list's chain of nodes
//to this
//
template <typename T>
void ForwardList<T>::appendList(const ForwardList<T>& other)
{
	//if other is empty, leave
	if (other.isEmpty())
		return;

	//a pointer to save the end of the new chain
	Node<T>* endOfChain = NULL;

	//clone other list's chain of nodes
	Node<T>* newChain = cloneChain(other.head, &endOfChain);

	//if the list is empty, attach the chain to the head pointer
	if (isEmpty())
	{
		this->head = newChain;
	}
	else //else attach it to the tail node
	{
		this->tail->next = newChain;
	}

	assert(endOfChain);

	//update tail
	this->tail = endOfChain;    

	//increase count
	this->count += other.count;
}

//
//returns the node before the passed one 
//
//if node is the actual head
//no matching node is found and NULL is returned
//
template <typename T>
Node<T>* ForwardList<T>::findNodeBefore(const Node<T>* node)const
{
	//start from head
	Node<T>* current = head;

	//while there is an actual node and its sucessor is not the one 
	//we are searching for, move forward in the list
	while (current && current->next != node)
	{
		current = current->next;
	}

	//if the list was empty, NULL is returned
	//if the passed node is not in the list,
	//the end of list is reached, where current points to NULL
	return current;
}





//------------------------------------------------------------------------------------
//
//CTORS
//


//
//the list is empty when constructed
//
template <typename T>
ForwardList<T>::ForwardList()
	:
	count(0),
	head(NULL),
	tail(NULL)
{
	;
}


//
//initialize members as 0
//and append other's chain of nodes 
//
template <typename T>
ForwardList<T>::ForwardList(const ForwardList<T>& other)
	:
	count(0),
	head(NULL),
	tail(NULL)
{
	appendList(other);
	assert(count == other.count);
}



template <typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& other)
{
	if (this != &other)
	{
		//copy other's chain in a temp. object
		ForwardList<T> temp(other);

		//swap this' chain with temp's chain
		std::swap(this->head, temp.head);
		std::swap(this->tail, temp.tail);
		
		//swap the count and leave temp destroy the old chain
		std::swap(this->count, temp.count);
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



//-------------------------------------------------------------------------------------
//
//INSERTION AND REMOVAL
//


//
//add a node with the passed value 
//as head node
//
template <typename T>
void ForwardList<T>::addAsHead(const T& value)
{
	//make a new node and set its successor 
	//to be the current head node
	Node<T>* newHead = new Node<T>(value, this->head);

	//if the list is emtpy, set tail
	if (isEmpty())
	{
		this->tail = newHead;
	}

	//update head
	this->head = newHead;

	//update count
	++count;
}


//
//add a node with the passed value
//as the new tail
//
template <typename T>
void ForwardList<T>::addAsTail(const T& value)
{
	//has no successor as a tail
	Node<T>* newTail = new Node<T>(value);

	//if the list is empty
	if (isEmpty())
	{
		//update head
		this->head = newTail;
	}
	else
	{	
		//update the successor of current tail
		this->tail->next = newTail;
	}

	//update tail
	this->tail = newTail;

	//update count
	++count;
}


//
//remove the head node
//
//if the list is empty, an exception is thrown
//
template <typename T>
void ForwardList<T>::removeHead()
{
	checkIsEmtpy();

	assert(head);

	//keep the address of the old head node
	Node<T>* oldHead = head;

	//move to the next node in the list
	head = head->next;

	//if this was the only node in the list
	if(head == NULL)
	{
		//update tail to be NULL too
		tail = NULL;
	}

	//free old head
	delete oldHead;

	//update count 
	--count;
}




//
//remove the specified node from the list
//
//if the pointer is NULL, the function does nothing
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

			//if the node we are about to delete is the tail 
			if (node == tail)
			{
				tail = nodeBefore;
			}

			//update the successor of the previous node
			nodeBefore->next = node->next;

			//free node
			delete node;

			//update count
			--count;
		}
	}
}


//
//if the iterator is not from this list
//an exception is thrown
//
template <typename T>
void ForwardList<T>::removeAt(ForwardListIterator<T>& it)
{
	if (it.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	//remove it
	removeAt(it.current);

	//invalidate iterator
	it.current = NULL;
}



template <typename T>
void ForwardList<T>::removeTail()
{
	return removeAt(tail);
}



//
//insert a node with the sent value
//exactly after the sent node
//
// \ if the pointer is NULL
//   or it points to the tail node
//   addAsTail is called 
//
template <typename T>
void ForwardList<T>::insertAfter(Node<T>* node, const T& data)
{
	//if NULL or tail
	if (!node || node == tail)
	{
		addAsTail(data);
	}
	else //else insert after it
	{
		//if it does not point to the tail (and is not NULL) it has a successor
		assert(node->hasSuccessor());

		//insert it exactly after the node
		Node<T>* newNode = new Node<T>(data, node->next);

		//update node's successor as the new node
		node->next = newNode;

		//increment count
		++count;
	}
}



//
//add a new node with the sent data
//exactly after the iterator's current
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::insertAfter(ForwardListIterator<T>& it, const T& data)
{
	if (it.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	//insert it
	insertAfter(it.current, data);
}




//
//insert a node with the sent data 
//exactly before the sent node
//
// \ if the pointer is NULL or points to the head
//    addAsHead is called
//
template <typename T>
void ForwardList<T>::insertBefore(Node<T>* node, const T& data)
{
	//if NULL or points to the head node, insert before the head
	if (!node || node == head)
	{
		addAsHead(data);
	}
	else
	{
		//it has a predecessor, because it is not the head node
		Node<T>* previous = findNodeBefore(node);
		
		assert(previous);

		//insert after its predecessor
		insertAfter(previous, data);
	}
}



//
//insert a new node with the sent data
//exactly before it.current
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::insertBefore(ForwardListIterator<T>& it, const T& data)
{
	if (it.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	//insert before it
	insertBefore(it.current, data);
}





//
//remove the node after the one the iterator points to
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::removeAfter(ForwardListIterator<T>& it)
{
	if (it.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	//remove the one after it
	removeAt(it.current->next);
}




//
//remove the node before the one the iterator points to
//
// \ if the iterator is not from this list
//   std::invalid_arg is thrown
//
template <typename T>
void ForwardList<T>::removeBefore(ForwardListIterator<T>& it)
{
	if (it.owner != this)
		throw std::invalid_argument("Invalid iterator passed!");

	//find the one before it 
	Node<T>* previous = findNodeBefore(it.current);

	//remove the one before it
	removeAt(previous);
}