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

		template <typename F>
		F superpose(F&&) = delete;

		template <typename F, typename... Gs>
		auto superpose(F f, Gs... funs)
		{
			return [f, funs...](const auto&... args) -> decltype(auto) { return f(funs(args...)...); };
		}

		template <typename F, typename G>
		auto compose(F f, G g)
		{
			return superpose(f, g);
		}

		template <typename F, typename G, typename... Gs>
		auto compose(F f, G g, Gs... funs)
		{
			return compose(compose(f, g), std::move(funs)...);
		}

		template <typename T>
		auto equalTo(T key)
		{
			return [lhs = std::move(key)](const auto& rhs) { return lhs == rhs; };
		}

		template <typename Key, typename KeyExtractor>
		auto matches(Key key, KeyExtractor extract)
		{
			auto wrap = [](auto&& key) { return Key(std::forward<decltype(key)>(key)); };
			return compose(equalTo(std::move(key)), wrap, extract);
		}
	}
}

#endif //__FUNCTIONAL_H_INCLUDED__