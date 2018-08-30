#ifndef __ITERATOR_WRAPPER_H_INCLUDED__
#define __ITERATOR_WRAPPER_H_INCLUDED__

#include "..\Type selector\BaseIteratorSelector.h"
#include <type_traits>

template <typename ForwardIterator, bool isConst = false>
class ForwardIteratorWrapper: public BaseIteratorSelector<isConst, typename ForwardIterator::value_type>::result
{
private:
	static_assert(std::is_same<typename ForwardIterator::iterator_category, std::forward_iterator_tag>::value ||
		          std::is_same<typename ForwardIterator::iterator_category, std::bidirectional_iterator_tag>::value, 
		          "template <class T> ForwardIteratorWrapper requires T to be a forward or bidirectional iterator");

	using pointer = typename ForwardIterator::pointer;
	using reference = typename ForwardIterator::reference;
	using value_type = typename ForwardIterator::value_type;
	using baseIterator = typename BaseIteratorSelector<isConst, value_type>::result;
	using baseIteratorPtr = std::unique_ptr<baseIterator>;

public:
	ForwardIteratorWrapper(const ForwardIterator& it);

	virtual reference operator*() const override;
	virtual pointer operator->() const override;
	virtual ForwardIteratorWrapper& operator++() override;
	virtual operator bool() const override;
	virtual bool operator!() const override;
	virtual baseIteratorPtr clone() const override;

	ForwardIteratorWrapper operator++(int);

private:
	ForwardIterator iterator;
};

#include "ForwardIteratorWrapperImpl.hpp"
#endif //__ITERATOR_WRAPPER_H_INCLUDED__