#ifndef __HASH_FUNC_H_INCLUDED__
#define __HASH_FUNC_H_INCLUDED__

#include "../String/String.h"

template <typename Key>
class HashFunction
{
public:
	unsigned operator()(const Key&)const;
};

template <typename Key>
inline unsigned HashFunction<Key>::operator()(const Key& key)const
{
	return static_cast<unsigned>(key);
}

#endif //__HASH_FUNC_H_INCLUDED__