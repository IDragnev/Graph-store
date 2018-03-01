#ifndef __HASH_H_INCLUDED__
#define __HASH_H_INCLUDED__

#include "../String/String.h"

template <typename Key>
class HashFunction
{
public:
	size_t operator()(const Key&)const;
};


template <typename Key>
inline size_t HashFunction<Key>::operator()(const Key& key)const
{
	return static_cast<size_t>(key);
}
#endif //__HASH_H_INCLUDED__