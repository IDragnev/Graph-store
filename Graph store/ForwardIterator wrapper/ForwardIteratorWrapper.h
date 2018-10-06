#ifndef __ITERATOR_WRAPPER_H_INCLUDED__
#define __ITERATOR_WRAPPER_H_INCLUDED__

#include "..\Traits\SelectBaseIterator.h"

template <typename ForwardIterator, typename EndType = std::false_type>
class ForwardIteratorWrapper: public BaseIteratorType<ForwardIterator>
{
private:
	static_assert(IsForwardIterator<ForwardIterator>::value, 
		          "template <class T> ForwardIteratorWrapper requires T to be a forward iterator");

	using pointer = typename std::iterator_traits<ForwardIterator>::pointer;
	using reference = typename std::iterator_traits<ForwardIterator>::reference;
	using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
	using BaseIterator = BaseIteratorType<ForwardIterator>;
	using BaseIteratorPtr = std::unique_ptr<BaseIterator>;

public:
	ForwardIteratorWrapper(const ForwardIterator& it, const EndType& end = {});

	reference operator*() const override;
	pointer operator->() const override;
	ForwardIteratorWrapper& operator++() override;
	operator bool() const override;
	bool operator!() const override;
	BaseIteratorPtr clone() const override;

	ForwardIteratorWrapper operator++(int);

private:
	ForwardIterator iterator;
	EndType end;
};


template <typename ForwardIterator, typename EndType = std::false_type>
auto makeWrapper(const ForwardIterator& it, const EndType& end = {})
{
	return std::make_unique<ForwardIteratorWrapper<ForwardIterator, EndType>>(it, end);
}

#include "ForwardIteratorWrapperImpl.hpp"
#endif //__ITERATOR_WRAPPER_H_INCLUDED__