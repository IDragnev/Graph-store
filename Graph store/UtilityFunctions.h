#ifndef __UTILITY_FUNCTIONS_H_INCLUDED__
#define __UTILITY_FUNCTIONS_H_INCLUDED__

#include <type_traits>
#include <iostream>

namespace IDragnev
{
	namespace Utility
	{
		template <typename T>
		inline constexpr auto asSigned(T x) noexcept { return std::make_signed_t<T>(x); }

		template <typename T>
		inline constexpr auto asUnsigned(T x) noexcept { return std::make_unsigned_t<T>(x); }

		template <typename T>
		inline constexpr decltype(auto) moveIfNothrowMoveAssignable(T& x) noexcept
		{
			using ReturnType =
				std::conditional_t<!std::is_nothrow_move_assignable_v<T> && std::is_copy_assignable_v<T>,
				const T&,
				T&&>;

			return static_cast<ReturnType>(x);
		}

		template <typename... Args>
		inline void print(std::ostream& out, const Args&... args)
		{
			(out << ... << args);
		}

		template <typename... Args>
		inline void print(const Args&... args)
		{
			print(std::cout, args...);
		}
	}
}

#endif //__UTILITY_FUNCTIONS_H_INCLUDED__