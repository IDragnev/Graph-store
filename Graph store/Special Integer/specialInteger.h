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
		using EnableIfFloatingPoint = std::enable_if_t<std::is_floating_point_v<T>>;

	public:
		constexpr SpecialInteger() noexcept;
		constexpr SpecialInteger(Integer value) noexcept;
		constexpr SpecialInteger(const SpecialInteger& source) = default;
		template <typename T, typename = EnableIfFloatingPoint<T>>
		SpecialInteger(T) = delete;

		template <typename T, typename = EnableIfFloatingPoint<T>>
		SpecialInteger<Integer>& operator=(T) = delete;
		constexpr SpecialInteger<Integer>& operator=(Integer value) noexcept;
		constexpr SpecialInteger<Integer>& operator=(const SpecialInteger& rhs) = default;

		static constexpr const SpecialInteger& Infinity() noexcept;

		constexpr SpecialInteger& operator+=(const SpecialInteger& rhs) noexcept;

		constexpr bool isEqualToInfinity() const noexcept;
		void print(std::ostream& os = std::cout) const;

		template <typename Integer>
		friend constexpr bool operator<(const SpecialInteger<Integer>& lhs, 
										const SpecialInteger<Integer>& rhs) noexcept;
		template <typename Integer>
		friend constexpr bool operator==(const SpecialInteger<Integer>& lhs, 
										 const SpecialInteger<Integer>& rhs) noexcept;

	private:
		Integer value;
		bool isInfinity;
	};

	template <typename Integer>
	constexpr bool operator>(const SpecialInteger<Integer>& lhs, 
							 const SpecialInteger<Integer>& rhs) noexcept;
	template <typename Integer>
	constexpr bool operator>=(const SpecialInteger<Integer>& lhs, 
							  const SpecialInteger<Integer>& rhs) noexcept;
	template <typename Integer>
	constexpr bool operator<=(const SpecialInteger<Integer>& lhs,
							  const SpecialInteger<Integer>& rhs) noexcept;
	template <typename Integer>
	constexpr bool operator!=(const SpecialInteger<Integer>& lhs,
							  const SpecialInteger<Integer>& rhs) noexcept;

	template <typename Integer>
	constexpr const SpecialInteger<Integer> operator+(const SpecialInteger<Integer>& lhs,
													  const SpecialInteger<Integer>& rhs) noexcept;

	template <typename Integer>
	std::ostream& operator<<(std::ostream& os, const SpecialInteger<Integer>& integer);
}

#include "SpecialIntegerImpl.hpp"
#endif //__SPECIAL_INTEGER_H_INCLUDED__
