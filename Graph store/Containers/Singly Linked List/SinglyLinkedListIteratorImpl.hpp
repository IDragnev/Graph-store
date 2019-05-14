
namespace IDragnev::Containers
{
	template <typename T>
	template <typename Item, bool isConst>
	inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::SinglyLinkedListIterator(NodePtr startNode, OwnerPtr owner) noexcept :
		current{ startNode },
		owner{ owner }
	{
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::SinglyLinkedListIterator(const SinglyLinkedListIterator<Item, false>& source) noexcept :
		SinglyLinkedListIterator{ source.current, source.owner }
	{
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::dereference() const -> Ref
	{
		assert(*this);
		return current->data;
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::increment() noexcept -> SinglyLinkedListIterator&
	{
		if (*this)
		{
			current = current->next;
		}

		return *this;
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline bool SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::isValid() const noexcept
	{
		return current != nullptr;
	}
}