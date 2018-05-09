#ifndef __CONST_ITERATOR_ABSTRACTION_H_INCLUDED__
#define __CONST_ITERATOR_ABSTRACTION_H_INCLUDED__

template <typename T>
class ConstIterator
{
public:
	virtual ~ConstIterator() = default;

	virtual const T& getCurrent() const = 0;
	virtual void goToNext() = 0;
	virtual bool isFinished() const = 0;
	virtual ConstIterator<T>* clone() const = 0;

protected:
	ConstIterator() = default;
	ConstIterator<T>& operator=(const ConstIterator<T>&) = default;
};

#endif //__CONST_ITERATOR_ABSTRACTION_H_INCLUDED__
