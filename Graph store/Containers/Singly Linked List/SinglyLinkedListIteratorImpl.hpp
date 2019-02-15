
namespace IDragnev
{
	namespace Containers
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
			return std::addressof(this->operator*());
		}

		template <typename T>
		template <typename Item, bool isConst>
		auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator++(int) noexcept -> SinglyLinkedListIterator
		{
			auto temp = *this;
			++(*this);

			return temp;
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline auto SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator++() noexcept -> SinglyLinkedListIterator&
		{
			if (this->operator bool())
			{
				current = current->next;
			}
			return *this;
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator bool() const noexcept
		{
			return current != nullptr;
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline bool SinglyLinkedList<T>::SinglyLinkedListIterator<Item, isConst>::operator!() const noexcept
		{
			return !(this->operator bool());
		}
	}
}