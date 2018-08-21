#ifndef __ITERATOR_ABSTRACTION_H_INCLUDED__
#define __ITERATOR_ABSTRACTION_H_INCLUDED__

#include <memory>

template <typename T>
class Iterator
{
public:
	virtual ~Iterator() = default;

	virtual T& operator*() const = 0;
	virtual T* operator->() const = 0;
	virtual Iterator<T>& operator++() = 0;
	virtual operator bool() const = 0;
	virtual bool operator!() const = 0;
	virtual std::unique_ptr<Iterator<T>> clone() const = 0;
};

#endif //__ITERATOR_ABSTRACTION_H_INCLUDED__
