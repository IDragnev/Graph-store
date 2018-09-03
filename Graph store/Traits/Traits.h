#ifndef __TRAITS_H_INCLUDED__
#define __TRAITS_H_INCLUDED__

#include <type_traits>
#include <iterator>

template <typename T>
struct is_const_reference
{
	using Type = typename std::remove_reference<T>::type;
	static constexpr bool value = typename std::is_const<Type>::value;
};


template <typename Iterator>
struct is_const_iterator
{
	using reference = typename std::iterator_traits<Iterator>::reference;
	static constexpr bool value = typename is_const_reference<reference>::value;
};


template <typename Iterator>
struct is_forward_iterator
{
	using category = typename std::iterator_traits<Iterator>::iterator_category;
	static constexpr bool value = typename std::is_convertible<category, std::forward_iterator_tag>::value;
};


#endif //__TRAITS_H_INCLUDED__