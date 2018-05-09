#ifndef __TYPE_SELECTOR_H_INCLUDED__
#define __TYPE_SELECTOR_H_INCLUDED__

template <bool isTrue, typename T, typename F>
struct typeSelector
{
	typedef T result;
};

template <typename T, typename F>
struct typeSelector<false, T, F>
{
	typedef F result;
};

#endif //__TYPE_SELECTOR_H_INCLUDED__
