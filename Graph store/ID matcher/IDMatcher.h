#ifndef __ID_MATHCER_H_INCLUDED__
#define __ID_MATHCER_H_INCLUDED__

#include "..\..\Third party\NamedType\named_type.hpp"
#include "..\Functional\Functional.h"
#include "..\IdGetter\IdGetter.h"
#include "..\String\String.h"

namespace IDragnev::GraphStore
{
	using ConstStringIDRef = fluent::NamedType<const String&, struct StringIdTag, fluent::Comparable>;

	inline auto matches(ConstStringIDRef ID)
	{
		using Functional::compose;
		using Functional::matches;

		auto keyExtractor = compose([](const auto& id) { return ConstStringIDRef{ id }; }, IdGetter{});

		return matches(ID, keyExtractor);
	}
}

#endif //__ID_MATHCER_H_INCLUDED__