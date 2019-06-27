#ifndef __FUNCTIONAL_H_INCLUDED__
#define __FUNCTIONAL_H_INCLUDED__

#include "Traits\Traits.h"

namespace IDragnev::Functional
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
		constexpr decltype(auto) operator()(T&& item) const noexcept
		{
			return std::forward<T>(item);
		}
	};

	struct EmptyFunction
	{
		template <typename... Args>
		constexpr void operator()(Args&&...) const noexcept { }
	};

	struct BackInserter
	{
		template <typename Container, typename T>
		decltype(auto) operator()(Container& c, T&& item) const noexcept(noexcept(c.insertBack(std::forward<T>(item))))
		{
			return c.insertBack(std::forward<T>(item));
		}
	};

	namespace Detail
	{
		template <typename... Ts>
		inline constexpr bool areNothrowCopyConstructible = Traits::allOf<std::is_nothrow_copy_constructible, Ts...>;
	}

	template <typename F>
	F superpose(F&&) = delete;

	template <typename F, typename... Gs>
	constexpr inline 
	auto superpose(F f, Gs... funs) noexcept(Detail::areNothrowCopyConstructible<F, Gs...>)
	{
		return [f, funs...](const auto&... args) constexpr mutable -> decltype(auto)
		{
			return f(funs(args...)...);
		};
	}

	template <typename F, typename G>
	constexpr inline 
	auto compose(F f, G g) noexcept(noexcept(superpose(f, g)))
	{
		return superpose(f, g);
	}

	template <typename F, typename G, typename... Gs>
	constexpr auto compose(F f, G g, Gs... funs) noexcept(noexcept(compose(f, g)))
	{
		return compose(compose(f, g), funs...);
	}

	template <typename T>
	inline auto equalTo(T key) noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		return [lhs = std::move(key)](const auto& rhs)
		{ 
			return lhs == rhs;
		};
	}

	template <typename Key, typename KeyExtractor>
	inline auto matches(Key key, KeyExtractor extractKey) 
	{
		return compose(equalTo(std::move(key)), std::move(extractKey));
	}

	template <typename T>
	inline auto plus(T rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		return[rhs = std::move(rhs)](auto&& lhs) -> decltype(auto)
		{
			using F = decltype(lhs);
			return std::forward<F>(lhs) + rhs;
		};
	}

	template <typename Predicate>
	constexpr inline 
	auto inverse(Predicate p) noexcept(std::is_nothrow_copy_constructible_v<Predicate>)
	{
		return [p](auto&&... args) constexpr noexcept(std::is_nothrow_invocable_v<decltype(p), decltype(args)...>)
		{
			return !p(std::forward<decltype(args)>(args)...);
		};
	}
}

#endif //__FUNCTIONAL_H_INCLUDED__