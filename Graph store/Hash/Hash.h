#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "../Dynamic Array/DArray.h"
#include "HashFunction.h"
#include <assert.h>

template <typename T>
struct Identity
{
	const T& operator()(const T& item) const { return item; }
};

template <
	typename Item,
	typename Key = Item,
	typename KeyAccessor = Identity<Key>,
	typename Hasher = HashFunction<Key> /* std::hash<key> ? */
	/*typename KeyEqual = std::equal<Key, Key>*/
> 
class Hash
{
private:
	using Table = DArray<Item*>;

public:
	using key_type = Key;
	using mapped_type = Item;
	using size_type = std::size_t;
	using hasher = Hasher;
	//using key_equal = KeyEqual;

public:
	Hash();
	explicit Hash(size_type expectedCount);
	template <typename InputIt>
	Hash(InputIt first, InputIt last);
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
	size_type getCount() const;

private:
	void swapContentsWithReconstructedParameter(Hash other);
	void enlarge();
	void shrink();
	void resize(size_type newSize);
	void toEmptyStateOfSize(size_type size);
	void insertAllItemsFrom(Table& table);

	long getPositionOfItemWithKey(const Key& key) const; 
	void rehashCluster(size_type start);
	Item* extractItemFromTableAt(size_type index);
	
	bool hasTooManyEmptySlots() const;
	bool canBeShrinked() const;
	bool isFillingUp() const;

	std::size_t computeHashValue(const Key& key) const;

private:
	static const size_type GROWTH_FACTOR = 2;
	static const size_type MIN_TABLE_SIZE = 3;
	static size_type calculateAppropriateSize(size_type expectedSize);

	static void nullify(Table& table);

private:
	size_type tableSize = MIN_TABLE_SIZE;
	size_type insertedCount{};
	Table table;
	mutable Hasher hashFunction;
	mutable KeyAccessor keyAccessor;
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
