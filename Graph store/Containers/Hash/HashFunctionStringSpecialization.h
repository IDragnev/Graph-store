#ifndef __HASH_FUNCTION_FOR_STRING__
#define __HASH_FUNCTION_FOR_STRING__

#include "..\..\String\String.h"
#include <functional>

/*
The Fowler–Noll–Vo hash function:
http://www.isthe.com/chongo/tech/comp/fnv/
*/

namespace std
{
	template <>
	struct hash<IDragnev::String>
	{
		std::size_t operator()(const IDragnev::String& key) const noexcept
		{
			const char* input = key;
			auto result = FNV_OFFSET_BASIS;

			while (*input)
			{
				result *= FNV_PRIME;
				result ^= *input;

				++input;
			}

			return result;
		}

	private:
		static const std::uint32_t FNV_PRIME = 16777619;
		static const std::uint32_t FNV_OFFSET_BASIS = 2166136261;
	};
}

#endif //__HASH_FUNCTION_FOR_STRING__