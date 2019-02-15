#ifndef __ITERATOR_WRAPPER_H_INCLUDED__
#define __ITERATOR_WRAPPER_H_INCLUDED__

#include "..\Traits\Traits.h"
#include "..\Iterator abstraction\Iterator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		namespace Detail
		{
			template <typename IteratorType>
			struct BaseIteratorSelector
			{
			private:
				static constexpr bool isConst = Traits::IsConstIterator<IteratorType>;
				using value_type = typename std::iterator_traits<IteratorType>::value_type;

			public:
				using type = std::conditional_t<isConst, ConstIterator<value_type>, Iterator<value_type>>;
			};
			
			template <typename Iterator>
			using BaseIterator = typename BaseIteratorSelector<Iterator>::type;
		}

		template <typename ForwardIterator, typename EndType = std::false_type>
		class ForwardIteratorAdaptor : public Detail::BaseIterator<ForwardIterator>
		{
		private:
			static_assert(Traits::IsForwardIterator<ForwardIterator>,
						  "template <class T> ForwardIteratorAdaptor requires T to be a forward iterator");

			using pointer = typename std::iterator_traits<ForwardIterator>::pointer;
			using reference = typename std::iterator_traits<ForwardIterator>::reference;
			using BaseIterator = Detail::BaseIterator<ForwardIterator>;
			using BaseIteratorPtr = std::unique_ptr<BaseIterator>;

		public:
			ForwardIteratorAdaptor(ForwardIterator it, EndType end = {});

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
		auto makeIteratorAdaptor(ForwardIterator it, EndType end = {})
		{
			return std::make_unique<ForwardIteratorAdaptor<ForwardIterator, EndType>>(it, end);
		}
	}
}

#include "ForwardIteratorAdaptorImpl.hpp"
#endif //__ITERATOR_WRAPPER_H_INCLUDED__