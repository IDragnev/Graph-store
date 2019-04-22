#ifndef __TRAITS_H_INCLUDED__
#define __TRAITS_H_INCLUDED__

#include <type_traits>
#include <iterator>

namespace IDragnev::Traits
{
	namespace Detail
	{
		template <typename Iterator>
		using IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category;

		template <typename Iterator>
		using IteratorReference = typename std::iterator_traits<Iterator>::reference;
	}

	template <typename T>
	struct IsConstReference : std::is_const<std::remove_reference_t<T>> { };
	
	template <typename T>
	inline constexpr bool isConstReference = IsConstReference<T>::value;

	template <typename Iterator>
	struct IsConstIterator : IsConstReference<Detail::IteratorReference<Iterator>> { };

	template <typename Iterator>
	inline constexpr bool isConstIterator = IsConstIterator<Iterator>::value;

	template <typename Iterator>
	struct IsForwardIterator : std::is_convertible<Detail::IteratorCategory<Iterator>, std::forward_iterator_tag> { };

	template <typename Iterator>
	inline constexpr bool isForwardIterator = IsForwardIterator<Iterator>::value;

	template <template <typename...> typename Predicate, typename... Ts>
	struct AllOf : std::bool_constant<(Predicate<Ts>::value && ...)> { };

	template <template <typename...> typename Predicate, typename... Ts>
	inline constexpr bool allOf = AllOf<Predicate, Ts...>::value;
}

#endif //__TRAITS_H_INCLUDED__