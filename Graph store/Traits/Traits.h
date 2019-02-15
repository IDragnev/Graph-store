#ifndef __TRAITS_H_INCLUDED__
#define __TRAITS_H_INCLUDED__

#include <type_traits>
#include <iterator>

namespace IDragnev
{
	namespace Traits
	{
		namespace Detail
		{
			template <typename T>
			struct IsConstReferenceImpl
			{
			private:
				using RawType = std::remove_reference_t<T>;

			public:
				static constexpr bool value = std::is_const<RawType>::value;
			};

			template <typename Iterator>
			struct IsConstIteratorImpl
			{
			private:
				using reference = typename std::iterator_traits<Iterator>::reference;

			public:
				static constexpr bool value = IsConstReferenceImpl<reference>::value;
			};

			template <typename Iterator>
			struct IsForwardIteratorImpl
			{
			private:
				using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

			public:
				static constexpr bool value = std::is_convertible<iterator_category, std::forward_iterator_tag>::value;
			};
		}

		template <typename T>
		inline constexpr bool IsConstReference = Detail::IsConstReferenceImpl<T>::value;

		template <typename T>
		inline constexpr bool IsConstIterator = Detail::IsConstIteratorImpl<T>::value;

		template <typename T>
		inline constexpr bool IsForwardIterator = Detail::IsForwardIteratorImpl<T>::value;
	}
}

#endif //__TRAITS_H_INCLUDED__