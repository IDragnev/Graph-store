#ifndef __PAIR_H_INCLUDED__
#define __PAIR_H_INCLUDED__

template <typename Item, typename Key>
struct Pair
{
	Pair();
	Pair(Item* itemPtr, const Key& key);
	Pair(Item* itemPtr, Key&& key);

	Item* itemPtr;
	Key key;
};

#include "PairImpl.hpp"
#endif //__PAIR_H_INCLUDED__