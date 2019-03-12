#ifndef __UTILITY_FUNCTIONS_H_INCLUDED__
#define __UTILITY_FUNCTIONS_H_INCLUDED__

#include <type_traits>

namespace IDragnev
{
	namespace Utility
	{
		template <typename T>
		constexpr auto asSigned(T x) noexcept { return std::make_signed_t<T>(x); }

		template <typename T>
		constexpr auto asUnsigned(T x) noexcept { return std::make_unsigned_t<T>(x); }

		struct LessThan
		{
			template <typename T, typename U>
			constexpr auto operator()(const T& lhs, const U& rhs) const noexcept(noexcept(lhs < rhs))
			{
				return lhs < rhs;
			}
		};

		struct EqualTo
		{
			template <typename T, typename U>
			constexpr auto operator()(const T& lhs, const U& rhs) const noexcept(noexcept(lhs == rhs))
			{
				return lhs == rhs;
			}
		};

		struct Identity
		{
			template <typename T>
			constexpr decltype(auto) operator()(T&& item) const noexcept { return std::forward<T>(item); }
		};

		struct EmptyFunction
		{
			template <typename... Args>
			constexpr void operator()(Args&&...) const noexcept { }
		};

		template <typename T>
		constexpr decltype(auto) moveIfNothrowMoveAssignable(T& x) noexcept
		{
			using ReturnType =
				std::conditional_t<!std::is_nothrow_move_assignable_v<T> && std::is_copy_assignable_v<T>,
				const T&,
				T&&>;

			return static_cast<ReturnType>(x);
		}

		template <typename... Args>
		void print(const Args&... args)
		{
			(std::cout << ... << args);
		}
	}
}

#endif //__UTILITY_FUNCTIONS_H_INCLUDED__