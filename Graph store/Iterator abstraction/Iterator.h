#ifndef __ITERATOR_ABSTRACTION_H_INCLUDED__
#define __ITERATOR_ABSTRACTION_H_INCLUDED__

#include <memory>

template <typename T>
class Iterator
{
public:
	virtual ~Iterator() = default;

	virtual T& getCurrent() const = 0;
	virtual void goToNext() = 0;
	virtual bool isFinished() const = 0;
	virtual std::unique_ptr<Iterator<T>> clone() const = 0;

protected:
	Iterator<T>& operator=(Iterator<T>&&) = default;
	Iterator<T>& operator=(const Iterator<T>&) = default;
};

#endif //__ITERATOR_ABSTRACTION_H_INCLUDED__
