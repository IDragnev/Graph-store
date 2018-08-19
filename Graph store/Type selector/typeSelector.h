#ifndef __TYPE_SELECTOR_H_INCLUDED__
#define __TYPE_SELECTOR_H_INCLUDED__

template <bool isTrue, typename T, typename F>
struct TypeSelector
{
	using result = T;
};

template <typename T, typename F>
struct TypeSelector<false, T, F>
{
	using result = F;
};

#endif //__TYPE_SELECTOR_H_INCLUDED__
