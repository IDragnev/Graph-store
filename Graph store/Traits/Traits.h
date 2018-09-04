#ifndef __TRAITS_H_INCLUDED__
#define __TRAITS_H_INCLUDED__

#include <type_traits>
#include <iterator>

template <typename T>
struct is_const_reference
{
	using RawType = std::remove_reference_t<T>;
	static constexpr bool value = std::is_const<RawType>::value;
};


template <typename Iterator>
struct is_const_iterator
{
	using reference = typename std::iterator_traits<Iterator>::reference;
	static constexpr bool value = is_const_reference<reference>::value;
};


template <typename Iterator>
struct is_forward_iterator
{
	using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
	static constexpr bool value = std::is_convertible<iterator_category, std::forward_iterator_tag>::value;
};


#endif //__TRAITS_H_INCLUDED__