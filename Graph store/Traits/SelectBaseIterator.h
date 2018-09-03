#ifndef __SELECT_BASE_ITERATOR_H_INCLUDED__
#define __SELECT_BASE_ITERATOR_H_INCLUDED__

#include "Traits.h"
#include "..\Iterator abstraction\Iterator.h"

template <typename IteratorType>
struct select_base_iterator
{
	static constexpr bool isConst = is_const_iterator<IteratorType>::value;
	using T = typename std::iterator_traits<IteratorType>::value_type;
	using type = std::conditional_t<isConst, ConstIterator<T>, Iterator<T>>;
};


template <typename IteratorType>
using select_base_iterator_t = typename select_base_iterator<IteratorType>::type;

#endif //__SELECT_BASE_ITERATOR_H_INCLUDED__
