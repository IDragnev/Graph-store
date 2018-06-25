#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DArray.h"
#include "HashFunction.h"
#include <assert.h>


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
	void swapContentsWithReconstructedParameter(Hash other);
	void resize(unsignedInteger newSize);
	void toEmptyStateOfSize(unsignedInteger size);
	void insertAllItemsFrom(DArray<Item*>& table);

	long getPositionOfItemWithKey(const Key& key) const; 
	void rehashCluster(unsignedInteger start);
	Item* extractItemFromTableAt(unsignedInteger index);
	
	bool hasTooManyEmptySlots() const;
	bool canBeShrinked() const;
	bool isFillingUp() const;

private:
	static const unsignedInteger GROWTH_FACTOR = 2;
	static const unsignedInteger MIN_TABLE_SIZE = 3;
	static unsignedInteger calculateAppropriateSize(unsignedInteger expectedSize);

	static void nullify(DArray<Item*>& table);

private:
	unsignedInteger tableSize;
	unsignedInteger insertedCount;
	DArray<Item*> table;

	mutable HashFunction<Key> hashFunction;
	mutable KeyAccessor keyAccessor;
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
