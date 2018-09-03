#ifndef __ITERATOR_WRAPPER_H_INCLUDED__
#define __ITERATOR_WRAPPER_H_INCLUDED__

#include "..\Traits\Traits.h"
#include "..\Traits\SelectBaseIterator.h"

template <typename ForwardIterator, typename EndType = std::false_type>
class ForwardIteratorWrapper: public select_base_iterator_t<ForwardIterator>
{
private:
	static_assert(is_forward_iterator<ForwardIterator>::value, 
		          "template <class T> ForwardIteratorWrapper requires T to be a forward iterator");

	using pointer = typename std::iterator_traits<ForwardIterator>::pointer;
	using reference = typename std::iterator_traits<ForwardIterator>::reference;
	using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
	using baseIterator = select_base_iterator_t<ForwardIterator>;
	using baseIteratorPtr = std::unique_ptr<baseIterator>;

public:
	ForwardIteratorWrapper(const ForwardIterator& it, const EndType& end = {});

	virtual reference operator*() const override;
	virtual pointer operator->() const override;
	virtual ForwardIteratorWrapper& operator++() override;
	virtual operator bool() const override;
	virtual bool operator!() const override;
	virtual baseIteratorPtr clone() const override;

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