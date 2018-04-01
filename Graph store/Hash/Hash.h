#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DArray.h"
#include "HashFunction.h"
#include <assert.h>

/*
The Hash class represents a hash table. It stores the Items' addresses only, NOT values.
It is a template of Item, Key and KeyAccessor.

The KeyAccessor template parameter must be a function or a Function object
whose purpose is to access the key from each Item.
Its operator()'s signature should be: 
const Key& operator()(const Item&); (optionally const)

In order to use the Hash class, the template class HashFunction
must be specialised for the corresponding (user-defined) Key class.
Its operator()'s signature should be:
unsigned operator()(const Key&); (optionally const)

The Key class must support operator==.

The HashFunction specialization and KeyAccessor's special members must not
throw exceptions.
HashFunction and KeyAccessor must be copy constructible and copy assignable.
*/


template <typename Item, typename Key, typename KeyAccessor>
class Hash
{
public:
	Hash(size_t expectedSize);
	Hash(Hash&& source);
	Hash(const Hash& source) = default;
	~Hash() = default;

	Hash& operator=(Hash&& rhs);
	Hash& operator=(const Hash& rhs);

	size_t getCount()const;
	bool isEmpty()const;

	void empty();

	void insert(Item& item);
	Item* remove(const Key& key);
	Item* search(const Key& key);

private:
	size_t tableSize;
	size_t insertedCount;
	DArray<Item*> table;
	HashFunction<Key> hashFunction;
	KeyAccessor keyAccessor;

private:
	static const size_t MIN_SIZE = 3;
	static size_t calculateAppropriateSize(size_t expectedSize);
	static bool isValidPosition(long index);

private:
	long getIndexByKey(const Key& key); 
	void resize(size_t newSize);
	void nullTable();
	void rehashCluster(size_t start);
	void swapContentsWithReconstructedParameter(Hash other);
	bool shouldHalveTable()const;
	bool shouldDoubleTable()const;
	Item* extractItemFromTable(size_t index);
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
