#ifndef __FUNCTIONAL_H_INCLUDED__
#define __FUNCTIONAL_H_INCLUDED__

namespace IDragnev
{
	namespace Functional
	{
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
	}
}

#endif //__FUNCTIONAL_H_INCLUDED__