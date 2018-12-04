
namespace IDragnev
{
	namespace GraphStore
	{
		template <typename ForwardIterator, typename EndType>
		ForwardIteratorWrapper<ForwardIterator, EndType>::ForwardIteratorWrapper(const ForwardIterator& it, const EndType& end)  
			noexcept(std::is_nothrow_copy_constructible_v<ForwardIterator> &&
					 std::is_nothrow_copy_constructible_v<EndType>) :
			iterator{ it },
			end{ end }
		{
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator*() const -> reference
		{
			return *iterator;
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator->() const -> pointer
		{
			return iterator.operator->();
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator++(int) -> ForwardIteratorWrapper
		{
			auto temp = *this;
			++(*this);

			return temp;
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorWrapper<ForwardIterator, EndType>::operator++() -> ForwardIteratorWrapper&
		{
			++iterator;
			return *this;
		}

		template <typename ForwardIterator, typename EndType>
		ForwardIteratorWrapper<ForwardIterator, EndType>::operator bool() const noexcept
		{
			return iterator != end;
		}

		template <typename ForwardIterator, typename EndType>
		bool ForwardIteratorWrapper<ForwardIterator, EndType>::operator!() const noexcept
		{
			return !(this->operator bool());
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorWrapper<ForwardIterator, EndType>::clone() const -> BaseIteratorPtr
		{
			return makeIteratorWrapper(iterator, end);
		}
	}
}