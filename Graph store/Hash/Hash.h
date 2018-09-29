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
	Hash();
	explicit Hash(std::size_t expectedCount);
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
	std::size_t getCount() const;

private:
	void swapContentsWithReconstructedParameter(Hash other);
	void enlarge();
	void shrink();
	void resize(std::size_t newSize);
	void toEmptyStateOfSize(std::size_t size);
	void insertAllItemsFrom(Table& table);

	long correspondingSlot(const Key& key) const; 
	void rehashCluster(std::size_t startingSlot);
	Item* extractSlotEntry(std::size_t slot);
	void insertAt(std::size_t slot, Item& item);

	bool hasTooManyEmptySlots() const;
	bool canBeShrinked() const;
	bool isFillingUp() const;

	std::size_t findFirstEmptySlotStartingAt(std::size_t slot) const;
	std::size_t computeHashValue(const Key& key) const;
	std::size_t followingSlot(std::size_t slot) const;
	bool isEmpty(std::size_t slot) const;

private:
	static const std::size_t GROWTH_FACTOR = 2;
	static const std::size_t MIN_TABLE_SIZE = 3;
	static std::size_t calculateAppropriateSize(std::size_t expectedCount);

	static Table makeEmptyTable(std::size_t size);
	static void nullify(Table& table);

private:
	std::size_t insertedCount{};
	Table table;
	mutable Hasher hashFunction;
	mutable KeyAccessor keyAccessor;
};

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
