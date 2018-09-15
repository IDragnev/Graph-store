#ifndef __SPECIAL_INTEGER_H_INCLUDED__
#define __SPECIAL_INTEGER_H_INCLUDED__

#include <iostream>
#include <type_traits>

template <typename Integer = int>
class SpecialInteger
{
private:
	static_assert(std::numeric_limits<Integer>::is_integer, "SpecialInteger<T> requires T to be an integer type");
	
	template <typename T>
	using enable_if_floating_point = std::enable_if_t<std::is_floating_point<T>::value>;

public:
	SpecialInteger();
	SpecialInteger(Integer value);
	SpecialInteger(const SpecialInteger<Integer>& source) = default;
	template <typename T, typename = enable_if_floating_point<T>>
	SpecialInteger(T) = delete;

	template <typename T, typename = enable_if_floating_point<T>>
	SpecialInteger<Integer>& operator=(T) = delete;
	SpecialInteger<Integer>& operator=(Integer newValue);
	SpecialInteger<Integer>& operator=(const SpecialInteger<Integer>& rhs) = default;

	static const SpecialInteger<Integer>& Infinity();

	SpecialInteger<Integer>& operator+=(const SpecialInteger<Integer>& rhs);

	bool isEqualToInfinity() const;
	void print(std::ostream& outputStream = std::cout) const;

	template <typename Integer>
	friend bool operator<(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);
	template <typename Integer>
	friend bool operator==(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);

private:
	Integer value;
	bool isInfinity;
};

template <typename Integer>
bool operator>(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);
template <typename Integer>
bool operator>=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);
template <typename Integer>
bool operator<=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);
template <typename Integer>
bool operator!=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);

template <typename Integer>
const SpecialInteger<Integer> operator+(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);

template <typename Integer>
std::ostream& operator<<(std::ostream& outputStream, const SpecialInteger<Integer>& integer);

#include "SpecialIntegerImpl.hpp"
#endif //__SPECIAL_INTEGER_H_INCLUDED__
