#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "Dynamic Array\DArray.h"
#include "Hash\HashFunction.h"

/*
The Hash class represents a hash table. It stores the Items' addresses only, NOT values.

The Hash class is a template of Item, Key and KeyExtractor.
The KeyExtractor template parameter must be a function or a Function object
whose purpose is to access the key from each Item.
Its operator()'s signature must be: 
const Key& operator()(const Item&)const;  ????????????????????????????????????????????????????

In order to use the Hash class, the template class HashFunction
must be specialised for the correspondig (user-defined) Key class.
*/


template <typename Item, typename Key, typename KeyExtractor>
class Hash
{
public:
	Hash(size_t size);

	int getCount()const;
	bool isEmpty()const;
	bool isFull()const;

	void insert(Item&);
	Item* remove(const Key&);
	Item* search(const Key&); //const?

private:
	unsigned count;
	DArray<Item*> table;
	HashFunction<Key> hashFunction;
	KeyExtractor keyExtractor;

private:
	int getIndexByKey(const Key&); //const?

	void expand();
	void shrink();
};

#endif //__MY_HASH_HEADER_INCLUDED__
