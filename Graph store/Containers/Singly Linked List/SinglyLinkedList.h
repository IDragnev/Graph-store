#ifndef __SINGLY_LINKED_LIST_H_INCLUDED__
#define __SINGLY_LINKED_LIST_H_INCLUDED__

#include <utility>
#include <assert.h>
#include <iterator>

namespace IDragnev::Containers
{
	template <typename T>
	class SinglyLinkedList
	{
	private:
		static_assert(std::is_copy_constructible_v<T>,
			          "SinglyLinkedList<T> requires T to be copy constructible");

		template <typename Item>
		struct Node
		{
			template <typename T>
			Node(T&& data, Node<Item>* next = nullptr) :
				next{ next },
				data{ std::forward<T>(data) }
			{
			}

			Node<Item>* next;
			Item data;
		};

		template <typename Item, bool isConst = false>
		class SinglyLinkedListIterator
		{
		private:
			friend class SinglyLinkedList<Item>;
			using OwnerPtr = const SinglyLinkedList<Item>*;
			using NodePtr = std::conditional_t<isConst, const Node<Item>*, Node<Item>*>;

		public:
			using value_type = Item;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			using reference = std::conditional_t<isConst, const Item&, Item&>;
			using pointer = std::conditional_t<isConst, const Item*, Item*>;

		public:
			SinglyLinkedListIterator(const SinglyLinkedListIterator<Item, false>& source) noexcept;

			pointer operator->() const;
			reference operator*() const;

			SinglyLinkedListIterator& operator++() noexcept;
			SinglyLinkedListIterator operator++(int) noexcept;

			operator bool() const noexcept;
			bool operator!() const noexcept;

			friend bool operator==(const SinglyLinkedListIterator& lhs, const SinglyLinkedListIterator& rhs) noexcept
			{
				return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
			}

			friend bool operator!=(const SinglyLinkedListIterator& lhs, const SinglyLinkedListIterator& rhs) noexcept
			{
				return !(lhs == rhs);
			}

		private:
			SinglyLinkedListIterator(NodePtr startNode, OwnerPtr owner) noexcept;

		private:
			NodePtr current;
			OwnerPtr owner;
		};

	public:
		using iterator = SinglyLinkedListIterator<T, false>;
		using const_iterator = SinglyLinkedListIterator<T, true>;

		SinglyLinkedList() noexcept;
		template <typename InputIt>
		SinglyLinkedList(InputIt first, InputIt last);
		SinglyLinkedList(std::initializer_list<T> source);
		SinglyLinkedList(SinglyLinkedList&& source) noexcept;
		SinglyLinkedList(const SinglyLinkedList& source);
		~SinglyLinkedList();

		SinglyLinkedList<T>& operator=(SinglyLinkedList&& rhs) noexcept;
		SinglyLinkedList<T>& operator=(const SinglyLinkedList& rhs);

	public:
		void insertFront(const T& item);
		void insertBack(const T& item);
		void insertAfter(iterator& position, const T& item);
		void insertBefore(iterator& position, const T& item);

		void removeFront();
		void removeBack();
		void removeAt(iterator& position);
		void removeBefore(iterator& position);
		void removeAfter(iterator& position);

		void appendList(SinglyLinkedList other) noexcept;

		void clear() noexcept;
		bool isEmpty() const noexcept;
		std::size_t getCount() const noexcept;
		const T& getFront() const;
		const T& getBack() const;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

	private:
		void swapContentsWith(SinglyLinkedList other) noexcept;
		void appendContentOf(SinglyLinkedList&& source);

		template <typename InputIt>
		void tryToBuildChain(InputIt first, InputIt last);
		template <typename InputIt>
		void buildChain(InputIt first, InputIt last);

		void clearCurrentChain() noexcept;
		void nullifyMembers() noexcept;

		void removeAt(Node<T>* node);
		void removeNonHead(Node<T>* node);
		void insertAfter(Node<T>* node, const T& item);
		void insertBefore(Node<T>* node, const T& item);
		Node<T>* findNodeBefore(const Node<T>* node) const;

		bool validateOwnershipOf(const iterator& it) const noexcept;

	private:
		std::size_t count;
		Node<T>* head;
		Node<T>* tail;
	};

	template <typename T>
	bool operator==(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs) noexcept(noexcept(std::declval<T>() == std::declval<T>()));
	template <typename T>
	bool operator!=(const SinglyLinkedList<T>& lhs, const SinglyLinkedList<T>& rhs) noexcept(noexcept(lhs == rhs));
}

#include "SinglyLinkedListImpl.hpp"
#include "SinglyLinkedListIteratorImpl.hpp"
#endif //__SINGLY_LINKED_LIST_H_INCLUDED__