#ifndef __SPECIAL_INTEGER_H_INCLUDED__
#define __SPECIAL_INTEGER_H_INCLUDED__

#include <iostream>
#include <type_traits>

namespace IDragnev
{
	template <typename Integer = int>
	class SpecialInteger
	{
	private:
		static_assert(std::numeric_limits<Integer>::is_integer,
					  "SpecialInteger<T> requires T to be an integer type");
		template <typename T>
		using EnableIfFloatingPoint = std::enable_if_t<std::is_floating_point<T>::value>;

	public:
		SpecialInteger();
		SpecialInteger(Integer value);
		SpecialInteger(const SpecialInteger& source) = default;
		template <typename T, typename = EnableIfFloatingPoint<T>>
		SpecialInteger(T) = delete;

		template <typename T, typename = EnableIfFloatingPoint<T>>
		SpecialInteger<Integer>& operator=(T) = delete;
		SpecialInteger<Integer>& operator=(Integer newValue);
		SpecialInteger<Integer>& operator=(const SpecialInteger& rhs) = default;

		static const SpecialInteger& Infinity();

		SpecialInteger& operator+=(const SpecialInteger& rhs);

		bool isEqualToInfinity() const;
		void print(std::ostream& os = std::cout) const;

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
	std::ostream& operator<<(std::ostream& os, const SpecialInteger<Integer>& integer);
}

#include "SpecialIntegerImpl.hpp"
#endif //__SPECIAL_INTEGER_H_INCLUDED__
