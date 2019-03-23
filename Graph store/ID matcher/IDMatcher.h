#ifndef __ID_MATHCER_H_INCLUDED__
#define __ID_MATHCER_H_INCLUDED__

#include "..\..\Third party\NamedType\named_type.hpp"
#include "..\Functional\Functional.h"
#include "..\String\String.h"
#include <type_traits>

namespace IDragnev
{
	namespace GraphStore
	{
		using ConstStringIDRef = fluent::NamedType<const String&, struct StringIdTag, fluent::Comparable>;

		inline auto getID = [](const auto& item)
		{
			using RawType = std::decay_t<decltype(item)>;

			if constexpr (std::is_pointer_v<RawType> ||
						  std::is_same_v<RawType, std::unique_ptr<Graph>>)
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
#endif //__ID_MATHCER_H_INCLUDED__