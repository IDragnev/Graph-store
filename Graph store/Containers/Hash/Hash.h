#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "..\Dynamic Array\DArray.h"
#include <assert.h>
#include <optional>

namespace IDragnev
{
	namespace Containers
	{
		struct Identity
		{
			template <typename T>
			const T& operator()(const T& item) const noexcept { return item; }
		};

		template <
			typename Item,
			typename Key = Item,
			typename KeyAccessor = Identity,
			typename HashFun = std::hash<Key>,
			typename EqualityPredicate = std::equal_to<Key>
		> class Hash
		{
		private:	
			static constexpr bool isItemPointerType = std::is_pointer_v<Item>;
			using Element = std::conditional_t<isItemPointerType, Item, std::optional<Item>>;
			using Table = DArray<Element>;

			class DirectSize
			{
			public:
				constexpr explicit DirectSize(std::size_t size) noexcept : value{ size } { }

				constexpr auto get() const noexcept { return value; }

			private:
				std::size_t value;
			};

		public:
			Hash();
			explicit Hash(std::size_t expectedCount);
			template <typename InputIt>
			Hash(InputIt first, InputIt last);
			Hash(Hash&& source);
			Hash(const Hash& source) = default;

			Hash& operator=(Hash&& rhs);
			Hash& operator=(const Hash& rhs) = default;

			void insert(const Item& item);
			void remove(const Key& key);
			Element search(const Key& key) const;

			Item& operator[](const Key& key);
			const Item& operator[](const Key& key) const;
			bool contains(const Key& key) const;

			void empty();
			bool isEmpty() const;
			std::size_t getCount() const;

		private:
			Hash(DirectSize);

			void swapContentsWithReconstructedParameter(Hash other);
			void enlarge();
			void emptySlotAndShrink(std::size_t slot);
			void shrink();
			void resize(std::size_t newSize);
			void insertAllItemsFrom(const Table& table);
			void insertIfNotEmpty(const Element& element);
			
			static const Item& itemOf(const Element& element);
			static Item itemOf(Element&& element);

			std::optional<std::size_t> correspondingSlot(const Key& key) const;
			bool matchesItem(const Key& key, const Element& item) const;

			void rehashClusterStartingAt(std::size_t startingSlot);
			Item extractItemAt(std::size_t slot);
			void fillSlot(std::size_t slot, const Item& item);

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
			static std::size_t calculateSize(std::size_t expectedCount);
			static Table makeEmptyTable(std::size_t size);
			static void nullify(Table& table);

		private:
			std::size_t count{};
			Table table;
			mutable HashFun hashFunction;
			mutable KeyAccessor keyAccessor;
			mutable EqualityPredicate equalityPredicate;
		};
	}
}

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
