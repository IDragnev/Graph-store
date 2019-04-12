
namespace IDragnev::Containers
{

	template <typename T>
	SinglyLinkedList<T>::SinglyLinkedList() noexcept :
		count{ 0 },
		head{ nullptr },
		tail{ nullptr }
	{
	}

	template <typename T>
	inline SinglyLinkedList<T>::SinglyLinkedList(std::initializer_list<T> source) :
		SinglyLinkedList(source.begin(), source.end())
	{
	}

	template <typename T>
	inline SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& source) :
		SinglyLinkedList<T>(source.cbegin(), source.cend())
	{
	}

	template <typename T>
	template <typename InputIt>
	SinglyLinkedList<T>::SinglyLinkedList(InputIt first, InputIt last) :
		SinglyLinkedList()
	{
		if (first != last)
		{
			tryToBuildChain(first, last);
		}
	}

	template <typename T>
	template <typename InputIt>
	void SinglyLinkedList<T>::tryToBuildChain(InputIt first, InputIt last)
	{
		try
		{
			buildChain(first, last);
		}
		catch (...)
		{
			clear();
			throw;
		}
	}

	template <typename T>
	template <typename InputIt>
	void SinglyLinkedList<T>::buildChain(InputIt first, InputIt last)
	{
		assert(!head);
		assert(first != last);

		head = new Node<T>{ *first++ };
		tail = head;
		count = 1;

		for (auto current = first; current != last; ++current)
		{
			tail->next = new Node<T>{ *current };
			tail = tail->next;
			++count;
		}
	}

	template <typename T>
	void SinglyLinkedList<T>::clear() noexcept
	{
		clearCurrentChain();
		nullifyMembers();
	}

	template <typename T>
	void SinglyLinkedList<T>::clearCurrentChain() noexcept
	{
		while (head)
		{
			auto oldHead = head;
			head = head->next;
			delete oldHead;
		}
	}

	template <typename T>
	inline void SinglyLinkedList<T>::nullifyMembers() noexcept
	{
		count = 0;
		head = tail = nullptr;
	}

	template <typename T>
	SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList&& source) noexcept :
		head{ source.head },
		tail{ source.tail },
		count{ source.count }
	{
		source.nullifyMembers();
	}

	template <typename T>
	SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList& rhs)
	{
		if (this != &rhs)
		{
			swapContentsWith(rhs);
		}

		return *this;
	}

	template <typename T>
	SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList&& rhs) noexcept
	{
		if (this != &rhs)
		{
			swapContentsWith(std::move(rhs));
		}

		return *this;
	}

	template <typename T>
	inline void SinglyLinkedList<T>::swapContentsWith(SinglyLinkedList temp) noexcept
	{
		std::swap(head, temp.head);
		std::swap(tail, temp.tail);
		std::swap(count, temp.count);
	}

	template <typename T>
	inline SinglyLinkedList<T>::~SinglyLinkedList()
	{
		clearCurrentChain();
	}

	template <typename T>
	inline void SinglyLinkedList<T>::appendList(SinglyLinkedList source) noexcept
	{
		if (!source.isEmpty())
		{
			appendContentOf(std::move(source));
			source.nullifyMembers();
		}
	}

	template <typename T>
	inline bool SinglyLinkedList<T>::isEmpty() const noexcept
	{
		return count == 0;
	}

	template <typename T>
	void SinglyLinkedList<T>::appendContentOf(SinglyLinkedList&& source)
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
	void SinglyLinkedList<T>::insertBack(const T& item)
	{
		auto newTail = new Node<T>{ item };

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
	void SinglyLinkedList<T>::insertFront(const T& item)
	{
		auto newHead = new Node<T>{ item, head };

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
			insertBack(item);
		}
		else
		{
			assert(node->next);
			auto newNode = new Node<T>{ item, node->next };
			node->next = newNode;
			++count;
		}
	}

	template <typename T>
	void SinglyLinkedList<T>::insertBefore(Node<T>* node, const T& item)
	{
		if (!node || node == head)
		{
			insertFront(item);
		}
		else
		{
			auto previous = findNodeBefore(node);
			assert(previous);
			insertAfter(previous, item);
		}
	}

	template <typename T>
	auto SinglyLinkedList<T>::findNodeBefore(const Node<T>* node) const -> Node<T>*
	{
		assert(node);

		auto current = head;

		while (current && current->next != node)
		{
			current = current->next;
		}

		return current;
	}

	template <typename T>
	void SinglyLinkedList<T>::removeFront()
	{
		assert(!isEmpty());

		auto oldHead = head;
		head = head->next;

		if (!head)
		{
			tail = nullptr;
		}

		delete oldHead;
		--count;
	}

	template <typename T>
	inline void SinglyLinkedList<T>::removeBack()
	{
		assert(!isEmpty());
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
			removeFront();
		}
		else
		{
			removeNonHead(node);
		}
	}

	template <typename T>
	void SinglyLinkedList<T>::removeNonHead(Node<T>* node)
	{
		auto nodeBefore = findNodeBefore(node);
		assert(nodeBefore);

		if (node == tail)
		{
			tail = nodeBefore;
		}

		nodeBefore->next = node->next;
		delete node;
		--count;
	}

	template <typename T>
	inline void SinglyLinkedList<T>::removeAt(iterator& it)
	{
		assert(validateOwnershipOf(it));
		removeAt(it.current);
		it.current = nullptr;
	}

	template <typename T>
	inline void SinglyLinkedList<T>::removeAfter(iterator& it)
	{
		assert(validateOwnershipOf(it));
		removeAt(it.current->next);
	}

	template <typename T>
	inline void SinglyLinkedList<T>::removeBefore(iterator& it)
	{
		assert(validateOwnershipOf(it));
		removeAt(findNodeBefore(it.current));
	}

	template <typename T>
	inline void SinglyLinkedList<T>::insertAfter(iterator& it, const T& item)
	{
		assert(validateOwnershipOf(it));
		insertAfter(it.current, item);
	}

	template <typename T>
	inline void SinglyLinkedList<T>::insertBefore(iterator& it, const T& item)
	{
		assert(validateOwnershipOf(it));
		insertBefore(it.current, item);
	}

	template <typename T>
	inline bool SinglyLinkedList<T>::validateOwnershipOf(const iterator& it) const noexcept
	{
		return it.owner == this;
	}

	template <typename T>
	inline const T& SinglyLinkedList<T>::getFront() const
	{
		assert(!isEmpty());
		return head->data;
	}

	template <typename T>
	inline const T& SinglyLinkedList<T>::getBack() const
	{
		assert(!isEmpty());
		return tail->data;
	}

	template <typename T>
	inline std::size_t SinglyLinkedList<T>::getCount() const noexcept
	{
		return count;
	}

	template <typename T>
	inline auto SinglyLinkedList<T>::cbegin() const noexcept -> const_iterator
	{
		return const_iterator{ head, this };
	}

	template <typename T>
	inline auto SinglyLinkedList<T>::cend() const noexcept -> const_iterator
	{
		return const_iterator{ nullptr, this };
	}

	template <typename T>
	inline auto SinglyLinkedList<T>::begin() noexcept -> iterator
	{
		return iterator{ head, this };
	}

	template <typename T>
	inline auto SinglyLinkedList<T>::end() noexcept -> iterator
	{
		return iterator{ nullptr, this };
	}

	template <typename T>
	inline auto SinglyLinkedList<T>::begin() const noexcept -> const_iterator
	{
		return cbegin();
	}

	template <typename T>
	inline auto SinglyLinkedList<T>::end() const noexcept -> const_iterator
	{
		return cend();
	}

	template <typename T>
	inline bool operator==(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs) noexcept(noexcept(std::declval<T>() == std::declval<T>()))
	{
		return std::equal(lhs.cbegin(), lhs.cend(),
			              rhs.cbegin(), rhs.cend());
	}

	template <typename T>
	inline bool operator!=(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs) noexcept(noexcept(lhs == rhs))
	{
		return !(lhs == rhs);
	}
}