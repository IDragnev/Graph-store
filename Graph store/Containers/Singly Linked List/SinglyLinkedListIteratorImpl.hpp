
namespace Containers
{
	template <typename T>
	template <typename Item, bool isConst>
	inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::SinglyLinkedListIterator(nodePtr startNode, ownerPtr owner) :
		current{ startNode },
		owner{ owner }
	{
	}


	template <typename T>
	template <typename Item, bool isConst>
	inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::SinglyLinkedListIterator(const SinglyLinkedListIterator<Item, false>& source) :
		SinglyLinkedListIterator<Item, isConst>{ source.current, source.owner }
	{
	}


	template <typename T>
	template <typename Item, bool isConst>
	inline auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator*() const -> reference
	{
		assert(this->operator bool());
		return current->data;
	}


	template <typename T>
	template <typename Item, bool isConst>
	inline auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator->() const -> pointer
	{
		return &(this->operator*());
	}


	template <typename T>
	template <typename Item, bool isConst>
	auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator++(int) -> SinglyLinkedListIterator
	{
		auto temp = *this;
		++(*this);

		return temp;
	}


	template <typename T>
	template <typename Item, bool isConst>
	inline auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator++() -> SinglyLinkedListIterator&
	{
		if (this->operator bool())
		{
			current = current->next;
		}
		return *this;
	}


	template <typename T>
	template <typename Item, bool isConst>
	inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator bool() const
	{
		return current != nullptr;
	}


	template <typename T>
	template <typename Item, bool isConst>
	inline bool SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator!() const
	{
		return !(this->operator bool());
	}


	template <typename T, bool isConst>
	inline bool operator==(typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& lhs,
		typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& rhs)
	{
		return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
	}


	template <typename T, bool isConst>
	inline bool operator!=(typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& lhs,
						   typename const SinglyLinkedList<T>::SinglyLinkedListIterator<T, isConst>& rhs)
	{
		return !(lhs == rhs);
	}
}