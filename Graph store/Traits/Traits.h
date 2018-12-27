#ifndef __TRAITS_H_INCLUDED__
#define __TRAITS_H_INCLUDED__

#include <type_traits>
#include <iterator>

namespace IDragnev
{
	namespace Traits
	{
		template <typename T>
		struct IsConstReference
		{
		private:
			using RawType = std::remove_reference_t<T>;

		public:
			static constexpr bool value = std::is_const<RawType>::value;
		};

		template <typename Iterator>
		struct IsConstIterator
		{
		private:
			using reference = typename std::iterator_traits<Iterator>::reference;

		public:
			static constexpr bool value = IsConstReference<reference>::value;
		};

		template <typename Iterator>
		struct IsForwardIterator
		{
		private:
			using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

		public:
			static constexpr bool value = std::is_convertible<iterator_category, std::forward_iterator_tag>::value;
		};
	}
}

#endif //__TRAITS_H_INCLUDED__