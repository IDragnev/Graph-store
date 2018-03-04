#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DArray.h"
#include "Hash\HashFunction.h"
#include <assert.h>

/*
The Hash class represents a hash table. It stores the Items' addresses only, NOT values.
It is a template of Item, Key and KeyExtractor.

The KeyExtractor template parameter must be a function or a Function object
whose purpose is to access the key from each Item.
Its operator()'s signature should be: 
const Key& operator()(const Item&); (optionally const)

In order to use the Hash class, the template class HashFunction
must be specialised for the correspondig (user-defined) Key class.
Its operator()'s signature should be:
size_t operator()(const Key&); (optionally const)

The Key class/struct must support operator==.
*/


template <typename Item, typename Key, typename KeyExtractor>
class Hash
{
public:
	Hash(int expectedSize);

	int getCount()const;
	bool isEmpty()const;
	bool isFull()const;

	void empty();

	void insert(Item&);
	Item* remove(const Key&);
	Item* search(const Key&);

private:
	int count;
	DArray<Item*> table;
	HashFunction<Key> hashFunction;
	KeyExtractor keyExtractor;

private:
	int getIndexByKey(const Key&); 
	void resize(int newSize);
	void nullTable();
	void rehashCluster(int start);
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
