#ifndef __SELECT_BASE_ITERATOR_H_INCLUDED__
#define __SELECT_BASE_ITERATOR_H_INCLUDED__

#include "Traits.h"
#include "..\Iterator abstraction\Iterator.h"

template <typename IteratorType>
struct SelectBaseIterator
{
private:
	static constexpr bool isConst = IsConstIterator<IteratorType>::value;
	using value_type = typename std::iterator_traits<IteratorType>::value_type;

public:
	using type = std::conditional_t<isConst, ConstIterator<value_type>, Iterator<value_type>>;
};


template <typename IteratorType>
using BaseIteratorType = typename SelectBaseIterator<IteratorType>::type;

#endif //__SELECT_BASE_ITERATOR_H_INCLUDED__
