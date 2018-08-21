#ifndef __CONST_ITERATOR_ABSTRACTION_H_INCLUDED__
#define __CONST_ITERATOR_ABSTRACTION_H_INCLUDED__

#include <memory>

template <typename T>
class ConstIterator
{
public:
	virtual ~ConstIterator() = default;

	virtual const T& operator*() const = 0;
	virtual const T* operator->() const = 0;
	virtual ConstIterator<T>& operator++() = 0;
	virtual operator bool() const = 0;
	virtual bool operator!() const = 0;
	virtual std::unique_ptr<ConstIterator<T>> clone() const = 0;
};

#endif //__CONST_ITERATOR_ABSTRACTION_H_INCLUDED__
