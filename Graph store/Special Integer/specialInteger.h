#ifndef __SPECIAL_INTEGER_H_INCLUDED__
#define __SPECIAL_INTEGER_H_INCLUDED__

#include <iostream>

template <typename Integer = int>
class specialInteger
{
	static_assert(std::numeric_limits<Integer>::is_integer, "specialInteger<T> requires T to be an integer arithmetic type");

public:
	specialInteger();
	specialInteger(Integer value);

	specialInteger<Integer>& operator=(Integer newValue);
	specialInteger<Integer>& operator=(const specialInteger<Integer>& rhs) = default;

	static const specialInteger<Integer>& getInfinity();

	specialInteger<Integer>& operator+=(const specialInteger<Integer>& rhs);

	bool isEqualToInfinity() const;
	void print(std::ostream& outputStream = std::cout) const;

	template <typename Integer>
	friend bool operator<(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs);
	template <typename Integer>
	friend bool operator==(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs);

private:
	static const specialInteger<Integer> MY_INFINITY;

private:
	Integer value;
	bool isInfinity;
};

template <typename Integer>
specialInteger<Integer> operator+(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs);

template <typename Integer>
std::ostream& operator<<(std::ostream& outputStream, const specialInteger<Integer>& integer);

template <typename Integer>
bool operator!=(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs);

#include "SpecialIntegerImpl.hpp"
#endif //__SPECIAL_INTEGER_H_INCLUDED__
