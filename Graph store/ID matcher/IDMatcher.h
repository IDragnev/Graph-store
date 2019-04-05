#ifndef __ID_MATHCER_H_INCLUDED__
#define __ID_MATHCER_H_INCLUDED__

#include "..\..\Third party\NamedType\named_type.hpp"
#include "..\Functional\Functional.h"
#include "..\String\String.h"

namespace IDragnev::GraphStore
{
	using ConstStringIDRef = fluent::NamedType<const String&, struct StringIdTag, fluent::Comparable>;

	class IdGetter
	{
	public:
		template <typename T>
		ConstStringIDRef operator()(const T& item) const
		{
			return ConstStringIDRef{ idOf(item) };
		}

	private:
		template <typename T>
		static auto idOf(const T& item) -> decltype(item->getID())
		{
			return item->getID();
		}

		template <typename T>
		static auto idOf(const T& item) -> decltype(item.getID())
		{
			return item.getID();
		}
	};

	inline auto matches(ConstStringIDRef ID)
	{
		return Functional::matches(ID, IdGetter{});
	}
}

#endif //__ID_MATHCER_H_INCLUDED__