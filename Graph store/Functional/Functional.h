#ifndef __FUNCTIONAL_H_INCLUDED__
#define __FUNCTIONAL_H_INCLUDED__

#include "..\Traits\Traits.h"

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
	inline auto superpose(F f, Gs... funs) noexcept(Detail::areNothrowCopyConstructible<F, Gs...>)
	{
		return[f, funs...](const auto&... args) mutable -> decltype(auto)
		{
			return f(funs(args...)...);
		};
	}

	template <typename F, typename G>
	inline auto compose(F f, G g) noexcept(noexcept(superpose(f, g)))
	{
		return superpose(f, g);
	}

	template <typename F, typename G, typename... Gs>
	inline auto compose(F f, G g, Gs... funs) noexcept(noexcept(compose(f, g)))
	{
		return compose(compose(f, g), funs...);
	}

	template <typename T>
	inline auto equalTo(T key)
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
	inline auto plus(T rhs)
	{
		return[rhs = std::move(rhs)](auto&& lhs)
		{
			using F = decltype(lhs);
			return std::forward<F>(lhs) + rhs;
		};
	}

	template <typename Predicate>
	inline auto inverse(Predicate p)
	{
		return [p](auto&&... args)
		{
			return !p(std::forward<decltype(args)>(args)...);
		};
	}
}

#endif //__FUNCTIONAL_H_INCLUDED__