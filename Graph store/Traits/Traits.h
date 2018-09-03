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
	using category = typename std::iterator_traits<Iterator>::iterator_category;
	static constexpr bool value = std::is_convertible<category, std::forward_iterator_tag>::value;
};


template <typename T>
struct as_pointer_to_const
{
	static_assert(std::is_pointer<T>::value, "template <class T> as_pointer_to_const requires T to be a pointer type");

	using RawType = std::remove_pointer_t<T>;
	using AsConst = std::add_const_t<RawType>;
	using type = std::add_pointer_t<AsConst>;	
};


template <typename T>
using as_pointer_to_const_t = typename as_pointer_to_const<T>::type;


template <typename Iterator>
struct is_pointer_iterator
{
	using value_type = typename std::iterator_traits<Iterator>::value_type;
	static constexpr bool value = std::is_pointer<value_type>::value;
};


#endif //__TRAITS_H_INCLUDED__