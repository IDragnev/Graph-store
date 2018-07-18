#ifndef __SPECIAL_INTEGER_H_INCLUDED__
#define __SPECIAL_INTEGER_H_INCLUDED__

#include <iostream>

template <typename Integer = int>
class SpecialInteger
{
	static_assert(std::numeric_limits<Integer>::is_integer, "SpecialInteger<T> requires T to be an integer arithmetic type");

public:
	SpecialInteger();
	SpecialInteger(Integer value);

	SpecialInteger<Integer>& operator=(Integer newValue);
	SpecialInteger<Integer>& operator=(const SpecialInteger<Integer>& rhs) = default;

	static const SpecialInteger<Integer>& getInfinity();

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
SpecialInteger<Integer> operator+(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);

template <typename Integer>
std::ostream& operator<<(std::ostream& outputStream, const SpecialInteger<Integer>& integer);

template <typename Integer>
bool operator!=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs);

#include "SpecialIntegerImpl.hpp"
#endif //__SPECIAL_INTEGER_H_INCLUDED__
