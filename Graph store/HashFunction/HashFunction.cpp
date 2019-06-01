#include "HashFunction.h"
#include "String\String.h"

using IDragnev::String;

namespace std
{
	std::size_t hash<String>::operator()(const String& key) const noexcept
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
}