#ifndef __ITERATOR_WRAPPER_H_INCLUDED__
#define __ITERATOR_WRAPPER_H_INCLUDED__

#include "..\Traits\SelectBaseIterator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		template <typename ForwardIterator, typename EndType = std::false_type>
		class ForwardIteratorAdaptor : public BaseIteratorType<ForwardIterator>
		{
		private:
			static_assert(Traits::IsForwardIterator<ForwardIterator>::value,
						  "template <class T> ForwardIteratorAdaptor requires T to be a forward iterator");

			using pointer = typename std::iterator_traits<ForwardIterator>::pointer;
			using reference = typename std::iterator_traits<ForwardIterator>::reference;
			using BaseIterator = BaseIteratorType<ForwardIterator>;
			using BaseIteratorPtr = std::unique_ptr<BaseIterator>;

		public:
			ForwardIteratorAdaptor(const ForwardIterator& it, const EndType& end = {}) noexcept(std::is_nothrow_copy_constructible_v<ForwardIterator> &&
																								std::is_nothrow_copy_constructible_v<EndType>);

			reference operator*() const override;
			pointer operator->() const override;
			ForwardIteratorAdaptor& operator++() override;
			operator bool() const noexcept override;
			bool operator!() const noexcept override;
			BaseIteratorPtr clone() const override;

			ForwardIteratorAdaptor operator++(int);

		private:
			ForwardIterator iterator;
			EndType end;
		};

		template <typename ForwardIterator, typename EndType = std::false_type>
		auto makeIteratorAdaptor(const ForwardIterator& it, const EndType& end = {})
		{
			return std::make_unique<ForwardIteratorAdaptor<ForwardIterator, EndType>>(it, end);
		}
	}
}

#include "ForwardIteratorAdaptorImpl.hpp"
#endif //__ITERATOR_WRAPPER_H_INCLUDED__