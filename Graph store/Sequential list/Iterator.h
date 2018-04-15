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

protected:
	Iterator<T>& operator=(const Iterator<T>&) = default;
};

#endif //__ITERATOR_ABSTRACTION_H_INCLUDED__
