#include "HashFunction.h"
#include "String\String.h"

using IDragnev::String;

namespace std
{
	std::size_t hash<String>::operator()(const String& key) const noexcept
	{
		auto input = static_cast<const char*>(key);
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