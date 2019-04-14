#ifndef __RANGES_H_INCLUDED__
#define __RANGES_H_INCLUDED__

#include "..\Functional\Functional.h"

namespace IDragnev::Ranges
{
	template <typename Iterator, typename Callable>
	void forEach(Iterator& iterator, Callable f)
	{
		while (iterator)
		{
			f(*iterator);
			++iterator;
		}
	}

	template <typename Iterator, typename Callable, typename Predicate>
	void forEachUntil(Iterator& iterator, Callable f, Predicate p)
	{
		while (iterator && !p(*iterator))
		{
			f(*iterator);
			++iterator;
		}
	}

	template <typename Iterator, typename Predicate>
	bool anyOf(Iterator& iterator, Predicate p)
	{
		while (iterator)
		{
			if (p(*iterator))
			{
				return true;
			}

			++iterator;
		}

		return false;
	}

	template <typename Iterator, typename Predicate>
	inline bool allOf(Iterator& iterator, Predicate p)
	{
		using Functional::inverse;
		return !anyOf(iterator, inverse(p));
	}
}

#endif // !__RANGES_H_INCLUDED__

