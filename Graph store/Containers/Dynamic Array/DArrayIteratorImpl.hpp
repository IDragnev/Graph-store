
namespace IDragnev
{
	namespace Containers
	{
		template <typename T>
		template <typename Item, bool isConst>
		inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(size_type startPosition, OwnerPtr owner) noexcept :
			current{ startPosition },
			owner{ owner }
		{
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(const DArrayIterator<Item, false>& source) noexcept :
			DArrayIterator{ source.current, source.owner }
		{
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline auto DArray<T>::DArrayIterator<Item, isConst>::operator*() const -> reference
		{
			assert(this->operator bool());
			return (*owner)[current];
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline auto DArray<T>::DArrayIterator<Item, isConst>::operator->() const -> pointer
		{
			return std::addressof(this->operator*());
		}

		template <typename T>
		template <typename Item, bool isConst>
		auto DArray<T>::DArrayIterator<Item, isConst>::operator++(int) -> DArrayIterator
		{
			auto temp = *this;
			++(*this);

			return temp;
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline auto DArray<T>::DArrayIterator<Item, isConst>::operator++() -> DArrayIterator&
		{
			++current;
			return *this;
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline DArray<T>::DArrayIterator<Item, isConst>::operator bool() const noexcept
		{
			return current < owner->getCount();
		}

		template <typename T>
		template <typename Item, bool isConst>
		inline bool DArray<T>::DArrayIterator<Item, isConst>::operator!() const noexcept
		{
			return !(this->operator bool());
		}
	}
}