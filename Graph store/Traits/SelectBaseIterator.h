#ifndef __SELECT_BASE_ITERATOR_H_INCLUDED__
#define __SELECT_BASE_ITERATOR_H_INCLUDED__

#include "Traits.h"
#include "..\Iterator abstraction\Iterator.h"
#include "..\Iterator abstraction\ConstIterator.h"

template <typename IteratorType>
struct select_base_iterator
{
	static constexpr bool isConst = typename is_const_iterator<IteratorType>::value;
	using T = typename std::iterator_traits<IteratorType>::value_type;
	using type = typename std::conditional<isConst, ConstIterator<T>, Iterator<T>>::type;
};

#endif //__SELECT_BASE_ITERATOR_H_INCLUDED__
