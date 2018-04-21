#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DArray.h"
#include "HashFunction.h"
#include <assert.h>

/*
The Hash class stores the Items' addresses only, NOT copies.

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
	typedef size_t sizeType;

public:
	explicit Hash(sizeType expectedCount);
	Hash(Hash&& source);
	Hash(const Hash& source) = default;
	~Hash() = default;

	Hash& operator=(Hash&& rhs);
	Hash& operator=(const Hash& rhs);

	sizeType getCount() const;
	bool isEmpty() const;

	void empty();

	void insert(Item& item);
	Item* remove(const Key& key);
	Item* search(const Key& key);

private:
	long searchTableAndGetIndex(const Key& key); 
	void resize(sizeType newSize);
	void nullTable();
	void rehashCluster(sizeType start);
	void swapContentsWithReconstructedParameter(Hash other);
	bool shouldHalveTable() const;
	bool shouldDoubleTable() const;
	Item* extractItemFromTableAt(sizeType index);

private:
	static const sizeType MIN_TABLE_SIZE = 3;
	static sizeType calculateAppropriateSize(sizeType expectedSize);
	static bool isValidSlot(long index);

private:
	sizeType tableSize;
	sizeType insertedCount;
	DArray<Item*> table;
	HashFunction<Key> hashFunction;
	KeyAccessor keyAccessor;
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
