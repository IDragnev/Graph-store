#ifndef __IS_INTEGER_H_INCLUDED__
#define __IS_INTEGER_H_INCLUDED__

#include <type_traits>

template <typename T>
struct IsInteger
{
	static constexpr bool value = std::numeric_limits<T>::is_integer && !std::is_same<T, bool>::value;
};

#endif //__IS_INTEGER_H_INCLUDED__