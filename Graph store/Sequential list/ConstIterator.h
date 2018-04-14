#ifndef __CONST_ITERATOR_INTERFACE_H_INCLUDED__
#define __CONST_ITERATOR_INTERFACE_H_INCLUDED__

template <typename T>
class ConstIterator
{
public:
	virtual ~ConstIterator() = default;

	virtual const T& getCurrent() const = 0;
	virtual void goToNext() = 0;
	virtual bool isFinished() const = 0;

	const T& operator*() const;

	ConstIterator<T>& operator++();

	bool operator!() const;
	operator bool() const;
};

#include "ConstIteratorImpl.hpp"
#endif // __CONST_ITERATOR_INTERFACE_H_INCLUDED__

