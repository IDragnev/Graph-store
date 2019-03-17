#ifndef __UTILITY_FUNCTIONS_H_INCLUDED__
#define __UTILITY_FUNCTIONS_H_INCLUDED__

#include <type_traits>
#include "..\Third party\NamedType\named_type.hpp"
#include "Functional\Functional.h"
#include "String\String.h"

namespace IDragnev
{
	namespace Utility
	{
		template <typename T>
		constexpr auto asSigned(T x) noexcept { return std::make_signed_t<T>(x); }

		template <typename T>
		constexpr auto asUnsigned(T x) noexcept { return std::make_unsigned_t<T>(x); }

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
		void print(std::ostream& out, const Args&... args)
		{
			(out << ... << args);
		}

		template <typename... Args>
		void print(const Args&... args)
		{
			print(std::cout, args...);
		}

		inline auto getID = [](const auto& item) -> decltype(auto)
		{
			using RawType = std::decay_t<decltype(item)>;

			if constexpr (std::is_pointer_v<RawType>)
			{
				return item->getID();
			}
			else
			{
				return item.getID();
			}
		};

		using ConstStringIDRef = fluent::NamedType<const String&, struct StringIdTag, fluent::Comparable>;
		
		auto matches(ConstStringIDRef ID) noexcept
		{
			using Functional::compose;
			using Functional::equalTo;
			return compose(equalTo(ID), [](const auto& id) { return ConstStringIDRef(id); }, getID);
		}
	}
}

#endif //__UTILITY_FUNCTIONS_H_INCLUDED__