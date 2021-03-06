#ifndef __MY_HASH_HEADER_INCLUDED__
#define __MY_HASH_HEADER_INCLUDED__

#include "Containers\Dynamic Array\DArray.h"
#include "Functional\Functional.h"
#include "Third party\NamedType\named_type.hpp"
#include "Traits\Traits.h"
#include <assert.h>
#include <optional>
#include <type_traits>

namespace IDragnev::Containers
{
	template <
		typename Item,
		typename Key = Item,
		typename KeyAccessor = Functional::Identity,
		typename HashFun = std::hash<Key>,
		typename EqualityPredicate = Functional::EqualTo
	> class Hash
	{
	private:
		template <typename Fn>
		inline static constexpr bool passesFunctorRequirements = std::is_nothrow_copy_assignable_v<Fn> &&
															     std::is_nothrow_copy_constructible_v<Fn> &&
																 std::is_nothrow_default_constructible_v<Fn>;
		static_assert(passesFunctorRequirements<KeyAccessor>,
			          "Hash requires KeyAccessor to be nothrow default-constructible, nothrow copy-constructible and nothrow copy-assignable");
		static_assert(passesFunctorRequirements<HashFun>,
			          "Hash requires HashFun to be nothrow default-constructible, nothrow copy-constructible and nothrow copy-assignable");
		static_assert(passesFunctorRequirements<EqualityPredicate>,
			          "Hash requires EqualityPredicate to be nothrow default-constructible, nothrow copy-constructible and nothrow copy-assignable");
		static_assert(std::is_nothrow_invocable_r_v<const Key&, const KeyAccessor, const Item&>,
			          "Hash requires KeyAccessor::operator()(const Item&) to be const and noexcept");
		static_assert(std::is_nothrow_invocable_r_v<std::size_t, const HashFun, const Key&>,
			          "Hash requires HashFun::operator()(const Key&) to be const and noexcept");
		static_assert(std::is_nothrow_invocable_r_v<bool, const EqualityPredicate, const Key&, const Key&>,
			          "Hash requires EqualityPredicate::operator()(const Key&, const Key&) to be const and noexcept");

		template <typename Iterator>
		using EnableIfInputIt = std::enable_if_t<Traits::isInputIterator<Iterator>>;
		using Element = std::conditional_t<std::is_pointer_v<Item>, Item, std::optional<Item>>;
		using Table = DArray<Element>;
		using DirectSize = fluent::NamedType<std::size_t, struct SizeTag>;

	public:
		Hash();
		explicit Hash(std::size_t expectedCount);
		Hash(std::initializer_list<Item> source);
		template <typename InputIt, typename = EnableIfInputIt<InputIt>>
		Hash(InputIt first, InputIt last);
		Hash(Hash&& source);
		Hash(const Hash& source) = default;

		Hash& operator=(Hash&& rhs);
		Hash& operator=(const Hash& rhs) = default;

		void insert(const Item& item);
		void insert(Item&& item);
		void remove(const Key& key);
		Element search(const Key& key) const noexcept;

		void clear();
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
		void insertAllItemsTo(Hash& other) const;

		template <typename T>
		static decltype(auto) itemOf(T&& element);

		std::optional<std::size_t> correspondingSlot(const Key& key) const noexcept;
		bool matches(const Key& key, const Element& item) const noexcept;

		void rehashClusterStartingAt(std::size_t startingSlot);
		Item extractItemAt(std::size_t slot);
		template <typename T>
		void fillSlot(std::size_t slot, T&& item);
		void empty(std::size_t slot) noexcept;

		bool hasTooManyEmptySlots() const noexcept;
		bool canBeShrinked() const noexcept;
		bool isFillingUp() const noexcept;

		std::size_t findSlotToInsert(const Item& item) const noexcept;
		std::size_t findFirstEmptySlotStartingAt(std::size_t slot) const noexcept;
		std::size_t computeHashValue(const Key& key) const noexcept;
		std::size_t followingSlot(std::size_t slot) const noexcept;
		bool isEmpty(std::size_t slot) const noexcept;

	private:
		static constexpr std::size_t GROWTH_FACTOR = 2;
		static constexpr std::size_t MIN_TABLE_SIZE = 3;
		static inline const Element nullEntry{};
		static std::size_t calculateSize(std::size_t expectedCount);

	private:
		std::size_t count = 0;
		Table table;
		HashFun hashFunction;
		KeyAccessor keyAccessor;
		EqualityPredicate equalityPredicate;
	};
}

#include "HashImpl.hpp"
#endif //__MY_HASH_HEADER_INCLUDED__
