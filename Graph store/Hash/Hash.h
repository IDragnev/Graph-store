#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "Dynamic Array\DArray.h"
#include "Hash\HashFunction.h"

template <typename Item, typename Key, typename KeyExtractor>
class Hash
{
public:
	Hash(size_t size);

	int getCount()const;
	bool isEmpty()const;
	bool isFull()const;

	void insert(const Item&);
	void remove(const Key&);
	Item* search(const Key&)const;

private:
	Item* items;
	int size;
	int count;
	HashFun hashFunction;
};

#endif //__MY_HASH_HEADER_INCLUDED__
