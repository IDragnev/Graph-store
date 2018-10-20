
namespace IDragnev
{
	namespace Containers
	{
		template <typename T>
		template <typename Item>
		SinglyLinkedList<T>::Node<Item>::Node(const Item& data, Node<Item>* next) :
			next{ next },
			data{ data }
		{
		}

		template <typename T>
		template <typename Item>
		SinglyLinkedList<T>::Node<Item>::Node(Item&& data, Node<Item>* next) :
			next{ next },
			data{ std::move(data) }
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
		inline void SinglyLinkedList<T>::tryToBuildChain(InputIt first, InputIt last)
		{
			try
			{
				buildChain(first, last);
			}
			catch (...)
			{
				empty();
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

			for (; first != last; ++first)
			{
				tail->next = new Node<T>{ *first };
				tail = tail->next;
				++count;
			}
		}

		template <typename T>
		void SinglyLinkedList<T>::empty()
		{
			clearCurrentChain();
			nullifyMembers();
		}

		template <typename T>
		void SinglyLinkedList<T>::clearCurrentChain()
		{
			while (head)
			{
				auto oldHead = head;
				head = head->next;
				delete oldHead;
			}
		}

		template <typename T>
		inline void SinglyLinkedList<T>::nullifyMembers()
		{
			count = 0;
			head = tail = nullptr;
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
		inline SinglyLinkedList<T>::~SinglyLinkedList()
		{
			clearCurrentChain();
		}

		template <typename T>
		inline void SinglyLinkedList<T>::appendList(const SinglyLinkedList<T>& source)
		{
			appendList(SinglyLinkedList<T>{ source });
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
		inline bool SinglyLinkedList<T>::isEmpty() const
		{
			return count == 0;
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
		void SinglyLinkedList<T>::insertAsHead(const T& item)
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
				insertAsTail(item);
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
				insertAsHead(item);
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
		void SinglyLinkedList<T>::removeHead()
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
		inline bool SinglyLinkedList<T>::validateOwnershipOf(const iterator& it) const
		{
			return it.owner == this;
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
		inline unsigned SinglyLinkedList<T>::getCount() const
		{
			return count;
		}

		template <typename T>
		inline auto SinglyLinkedList<T>::cbegin() const -> const_iterator
		{
			return const_iterator{ head, this };
		}

		template <typename T>
		inline auto SinglyLinkedList<T>::cend() const -> const_iterator
		{
			return const_iterator{ nullptr, this };
		}

		template <typename T>
		inline auto SinglyLinkedList<T>::begin() -> iterator
		{
			return iterator{ head, this };
		}

		template <typename T>
		inline auto SinglyLinkedList<T>::end() -> iterator
		{
			return iterator{ nullptr, this };
		}

		template <typename T>
		inline auto SinglyLinkedList<T>::begin() const -> const_iterator
		{
			return cbegin();
		}

		template <typename T>
		inline auto SinglyLinkedList<T>::end() const -> const_iterator
		{
			return cend();
		}

		template <typename T>
		inline bool operator==(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs)
		{
			return std::equal(lhs.cbegin(), lhs.cend(),
							  rhs.cbegin(), rhs.cend());
		}

		template <typename T>
		inline bool operator!=(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs)
		{
			return !(lhs == rhs);
		}
	}
}