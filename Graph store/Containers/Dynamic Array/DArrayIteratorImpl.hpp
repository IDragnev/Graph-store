
namespace IDragnev::Containers
{
	template <typename T>
	template <typename Item, bool isConst>
	inline DArray<T>::DArrayIterator<Item, isConst>::DArrayIterator(std::size_t startPosition, OwnerPtr owner) noexcept :
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
	inline auto DArray<T>::DArrayIterator<Item, isConst>::dereference() const -> Ref
	{
		assert(this->operator bool());
		return (*owner)[current];
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline auto DArray<T>::DArrayIterator<Item, isConst>::increment() -> DArrayIterator&
	{
		++current;
		return *this;
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline auto DArray<T>::DArrayIterator<Item, isConst>::decrement() -> DArrayIterator&
	{
		assert(current > 0u);
		--current;

		return *this;
	}

	template <typename T>
	template <typename Item, bool isConst>
	inline bool DArray<T>::DArrayIterator<Item, isConst>::isValid() const noexcept
	{
		return current < owner->getCount();
	}
}