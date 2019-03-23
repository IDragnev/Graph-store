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

		template <typename Tag>
		using ConstStringRef = fluent::NamedType<const String&, Tag, fluent::Comparable>;
		
		using ConstStringIDRef = ConstStringRef<struct StringIdTag>;
		
		inline auto getID = [](const auto& item)
		{
			using RawType = std::decay_t<decltype(item)>;
			
			if constexpr (std::is_pointer_v<RawType> || 
				          std::is_same_v<RawType, std::unique_ptr<GraphStore::Graph>>)
			{
				return ConstStringIDRef{ item->getID() };
			}
			else
			{
				return ConstStringIDRef{ item.getID() };
			}
		};

		auto matches(ConstStringIDRef ID)
		{
			return Functional::matches(ID, getID);
		}
	}
}

#endif //__UTILITY_FUNCTIONS_H_INCLUDED__