
namespace IDragnev
{
	namespace GraphStore
	{
		template <typename ForwardIterator, typename EndType>
		ForwardIteratorAdaptor<ForwardIterator, EndType>::ForwardIteratorAdaptor(ForwardIterator it, EndType end) :
			iterator{ it },
			end{ end }
		{
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorAdaptor<ForwardIterator, EndType>::operator*() const -> reference
		{
			return *iterator;
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorAdaptor<ForwardIterator, EndType>::operator->() const -> pointer
		{
			return iterator.operator->();
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorAdaptor<ForwardIterator, EndType>::operator++(int) -> ForwardIteratorAdaptor
		{
			auto temp = *this;
			++(*this);

			return temp;
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorAdaptor<ForwardIterator, EndType>::operator++() -> ForwardIteratorAdaptor&
		{
			++iterator;
			return *this;
		}

		template <typename ForwardIterator, typename EndType>
		ForwardIteratorAdaptor<ForwardIterator, EndType>::operator bool() const noexcept
		{
			return iterator != end;
		}

		template <typename ForwardIterator, typename EndType>
		bool ForwardIteratorAdaptor<ForwardIterator, EndType>::operator!() const noexcept
		{
			return !(this->operator bool());
		}

		template <typename ForwardIterator, typename EndType>
		auto ForwardIteratorAdaptor<ForwardIterator, EndType>::clone() const -> BaseIteratorPtr
		{
			return makeIteratorAdaptor(iterator, end);
		}
	}
}