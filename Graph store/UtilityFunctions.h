#ifndef __UTILITY_FUNCTIONS_H_INCLUDED__
#define __UTILITY_FUNCTIONS_H_INCLUDED__

#include <type_traits>

namespace IDragnev
{
	template <typename T>
	constexpr auto asSigned(T x) noexcept { return std::make_signed_t<T>(x); }

	template <typename T> 
	constexpr auto asUnsigned(T x) noexcept { return std::make_unsigned_t<T>(x); }

	template <typename T>
	using AssignmentReference =
		std::conditional_t<!std::is_nothrow_move_assignable_v<T> && std::is_copy_assignable_v<T>, const T&, T&&>;

	template <typename T>
	constexpr AssignmentReference<T> moveIfNothrowMoveAssignable(T& x) noexcept
	{
		return static_cast<AssignmentReference<T>>(x);
	}
}

#endif //__UTILITY_FUNCTIONS_H_INCLUDED__