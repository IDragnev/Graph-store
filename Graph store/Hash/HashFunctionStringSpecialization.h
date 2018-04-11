#ifndef __HASH_FUNCTION_FOR_STRING__
#define __HASH_FUNCTION_FOR_STRING__

#include "HashFunction.h"
#include "../String/String.h"

/*
The Fowler–Noll–Vo hash function:
http://www.isthe.com/chongo/tech/comp/fnv/
*/

template <>
class HashFunction<String>
{
public:
	unsigned operator()(const String& key)const;

private:
	static const unsigned FNV_PRIME = 16777619;
	static const unsigned FNV_OFFSET_BASIS = 2166136261;
};


unsigned HashFunction<String>::operator()(const String& key)const
{
	const char* input = key;

	unsigned hashValue = FNV_OFFSET_BASIS;

	while (*input)
	{
		hashValue *= FNV_PRIME;
		hashValue ^= *input;

		++input;
	}

	return hashValue;
}

#endif //__HASH_FUNCTION_FOR_STRING__