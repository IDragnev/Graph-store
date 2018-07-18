#ifndef __TYPE_SELECTOR_H_INCLUDED__
#define __TYPE_SELECTOR_H_INCLUDED__

template <bool isTrue, typename T, typename F>
struct TypeSelector
{
	typedef T result;
};

template <typename T, typename F>
struct TypeSelector<false, T, F>
{
	typedef F result;
};

#endif //__TYPE_SELECTOR_H_INCLUDED__
