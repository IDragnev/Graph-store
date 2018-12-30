#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "..\Dynamic Array\DArray.h"
#include "..\..\UtilityFunctions.h"
#include <assert.h>
#include <optional>
#include <type_traits>

namespace IDragnev
{
	namespace Containers
	{
		template <
			typename Item,
			typename Key = Item,
			typename KeyAccessor = Identity,
			typename HashFun = std::hash<Key>,
			typename EqualityPredicate = EqualTo
		> class Hash
		{
		private:
			template <typename Fn>
			struct FunctorRequirements
			{
			private:
				static constexpr bool hasNothrowDefaultCtor = std::is_nothrow_default_constructible_v<Fn>;
				static constexpr bool hasNothrowCopyCtor = std::is_nothrow_copy_constructible_v<Fn>;
				static constexpr bool hasNothrowCopyAssignment = std::is_nothrow_copy_assignable_v<Fn>;

			public:
				static constexpr bool value = hasNothrowCopyAssignment && hasNothrowCopyCtor && hasNothrowDefaultCtor;
			};

			template <typename Fn>
			inline static constexpr bool passesFunctorRequirements = FunctorRequirements<Fn>::value;

			static_assert(passesFunctorRequirements<KeyAccessor>,
						  "Hash requires KeyAccessor to be nothrow default-constructible, nothrow copy-constructible and nothrow copy-assignable");
			static_assert(passesFunctorRequirements<HashFun>,
						  "Hash requires HashFun to be nothrow default-constructible, nothrow copy-constructible and nothrow copy-assignable");
			static_assert(passesFunctorRequirements<EqualityPredicate>,
						  "Hash requires EqualityPredicate to be nothrow default-constructible, nothrow copy-constructible and nothrow copy-assignable");
			static_assert(std::is_nothrow_invocable_r_v<const Key&, KeyAccessor, const Item&>, 
						  "Hash requires KeyAccessor::operator()(const Item&) to be noexcept");
			static_assert(std::is_nothrow_invocable_r_v<std::size_t, HashFun, const Key&>,
						  "Hash requires HashFun::operator()(const Key&) to be noexcept");
			static_assert(std::is_nothrow_invocable_r_v<bool, EqualityPredicate, const Key&, const Key&>,
						  "Hash requires EqualityPredicate::operator()(const Key&, const Key&) to be noexcept");
			
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
			void insert(Item&& item);
			void remove(const Key& key);
			Element search(const Key& key) const noexcept;

			void empty();
			bool isEmpty() const noexcept;
			std::size_t getCount() const noexcept;

		private:
			Hash(DirectSize);

			void swapContentsWith(Hash other);
			void enlargeIfFillingUp();
			void enlarge();
			void emptySlotAndShrink(std::size_t slot);
			void shrink();
			void resize(std::size_t newSize);
			void insertAllItemsFrom(const Table& table);
			void insertIfNotEmpty(const Element& element);		
			static const Item& itemOf(const Element& element);

			std::optional<std::size_t> correspondingSlot(const Key& key) const noexcept;
			bool matchesItem(const Key& key, const Element& item) const noexcept;

			void rehashClusterStartingAt(std::size_t startingSlot);
			Item extractItemAt(std::size_t slot);
			template <typename T>
			void fillSlot(std::size_t slot, T&& item);

			bool hasTooManyEmptySlots() const noexcept;
			bool canBeShrinked() const noexcept;
			bool isFillingUp() const noexcept;

			std::size_t findSlotToInsert(const Item& item) const noexcept;
			std::size_t findFirstEmptySlotStartingAt(std::size_t slot) const noexcept;
			std::size_t computeHashValue(const Key& key) const noexcept;
			std::size_t followingSlot(std::size_t slot) const noexcept;
			bool isEmpty(std::size_t slot) const noexcept;

		private:
			static const std::size_t GROWTH_FACTOR = 2;
			static const std::size_t MIN_TABLE_SIZE = 3;
			static std::size_t calculateSize(std::size_t expectedCount);
			static Table makeEmptyTable(std::size_t size);

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
