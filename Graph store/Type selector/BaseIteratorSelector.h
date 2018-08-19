#ifndef __SELECT_ITERATOR_BASE_H_INCLUDED__
#define __SELECT_ITERATOR_BASE_H_INCLUDED__

#include "TypeSelector.h"
#include "../Iterator abstraction/ConstIterator.h"
#include "../Iterator abstraction/Iterator.h"

template <bool isConst, typename T>
struct BaseIteratorSelector
{
	using result = typename TypeSelector<isConst, ConstIterator<T>, Iterator<T>>::result;
};

#endif //__SELECT_ITERATOR_BASE_H_INCLUDED__
