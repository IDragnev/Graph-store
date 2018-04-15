#ifndef __ITERATOR_ABSTRACTION_H_INCLUDED__
#define __ITERATOR_ABSTRACTION_H_INCLUDED__

template <typename T>
class Iterator
{
public:
	virtual ~Iterator() = default;

	virtual T& getCurrent() = 0;
	virtual void goToNext() = 0;
	virtual bool isFinished() const = 0;
	virtual Iterator* clone() const = 0;

	T& operator*();

	Iterator<T>& operator++();

	bool operator!() const;
	operator bool() const;
};

#include "IteratorImpl.hpp"
#endif //__ITERATOR_ABSTRACTION_H_INCLUDED__
