#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DArray.h"
#include "HashFunction.h"
#include <assert.h>

/*
The KeyAccessor template parameter must be a function or a Function object
whose purpose is to access the key from each Item.
Its operator()'s signature should be: 
const Key& operator()(const Item&); (optionally const)

In order to use the Hash class, the template class HashFunction
must be specialised for the corresponding (user-defined) Key class.
Its operator()'s signature should be:
size_t operator()(const Key&); (optionally const)

The Key class must support operator==.

The HashFunction specialization and KeyAccessor's special members must not
throw exceptions.
HashFunction and KeyAccessor must be copy constructible and copy assignable.
*/


template <typename Item, typename Key, typename KeyAccessor>
class Hash
{
	typedef size_t unsignedInteger;

public:
	explicit Hash(unsignedInteger expectedCount);
	Hash(Hash&& source);
	Hash(const Hash& source) = default;
	~Hash() = default;

	Hash& operator=(Hash&& rhs);
	Hash& operator=(const Hash& rhs);

	void insert(Item& item);
	Item* remove(const Key& key);
	Item* search(const Key& key);
	const Item* search(const Key& key) const;

	void empty();
	bool isEmpty() const;
	unsignedInteger getCount() const;

private:
	void resize(unsignedInteger newSize);
	void swapContentsWithReconstructedParameter(Hash other);
	void nullifyTable();
	long searchTableAndGetIndex(const Key& key) const; 
	void rehashCluster(unsignedInteger start);
	Item* extractItemFromTableAt(unsignedInteger index);
	bool hasTooManyEmptySlots() const;
	bool canBeHalved() const;
	bool isFillingUp() const;

private:
	static const unsignedInteger MIN_TABLE_SIZE = 3;
	static unsignedInteger calculateAppropriateSize(unsignedInteger expectedSize);

private:
	unsignedInteger tableSize;
	unsignedInteger insertedCount;
	DArray<Item*> table;

	mutable HashFunction<Key> hashFunction;
	mutable KeyAccessor keyAccessor;
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
