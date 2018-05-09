#ifndef __SELECT_ITERATOR_BASE_H_INCLUDED__
#define __SELECT_ITERATOR_BASE_H_INCLUDED__

#include "typeSelector.h"
#include "../Iterator abstraction/ConstIterator.h"
#include "../Iterator abstraction/Iterator.h"

template <bool isConst, typename T>
struct selectBaseIterator
{
	typedef typename typeSelector<isConst, ConstIterator<T>, Iterator<T>>::result result;
};

#endif //__SELECT_ITERATOR_BASE_H_INCLUDED__
