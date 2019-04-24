#ifndef __UTILITY_FUNCTIONS_H_INCLUDED__
#define __UTILITY_FUNCTIONS_H_INCLUDED__

#include <type_traits>
#include <iostream>

namespace IDragnev::Utility
{
	template <typename T>
	inline constexpr auto asSigned(T x) noexcept
	{
		return std::make_signed_t<T>(x);
	}

	template <typename T>
	inline constexpr auto asUnsigned(T x) noexcept
	{
		return std::make_unsigned_t<T>(x);
	}

	template <typename Enum>
	inline constexpr auto toUnderlyingType(Enum x) noexcept
	{
		static_assert(std::is_enum_v<Enum>, "Non-enum type passed");
		return static_cast<std::underlying_type_t<Enum>>(x);
	}

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
	void print(std::ostream& out, const Args&... args)
	{
		(out << ... << args);
	}

	template <typename... Args>
	void print(const Args&... args)
	{
		print(std::cout, args...);
	}

	template <typename Callable>
	class CallOnDestruction
	{
	private:
		static_assert(std::is_nothrow_invocable_v<Callable>);
		static_assert(std::is_nothrow_move_constructible_v<Callable>);

	public:
		CallOnDestruction(Callable f) noexcept :
			f{ std::move(f) }
		{
		}
		~CallOnDestruction() { f(); }

		CallOnDestruction(const CallOnDestruction&) = delete;
		CallOnDestruction& operator=(const CallOnDestruction&) = delete;

	private:
		Callable f;
	};
}

#endif //__UTILITY_FUNCTIONS_H_INCLUDED__