#ifndef __HASH_FUNC_H_INCLUDED__
#define __HASH_FUNC_H_INCLUDED__

template <typename Key>
class HashFunction
{
public:
	std::size_t operator()(const Key& key) const
	{
		return static_cast<std::size_t>(key);
	}
};

#include "HashFunctionStringSpecialization.h"
#endif //__HASH_FUNC_H_INCLUDED__