#include "CppUnitTest.h"
#include "../../Graph store/String/String.h"
#include "../../Graph store/Containers/Hash/Hash.h"
#include "../../Graph store/Containers/Hash/HashFunctionStringSpecialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Containers::DArray;
using IDragnev::String;
using IDragnev::Containers::Hash;
using std::begin;
using std::end;

namespace HashTest
{
	struct Item
	{
		Item(const String& str) : key{ str } {}
		String key{};
	};

	bool operator==(const Item& lhs, const Item& rhs) noexcept { return lhs.key == rhs.key; }
	bool operator!=(const Item& lhs, const Item& rhs) noexcept { return !(lhs == rhs); }

	struct KeyExtractor
	{
		const String& operator()(const Item& item) const noexcept { return item.key; }
	};

	struct PointerKeyExtractor
	{
		const String& operator()(const Item* item) const noexcept { return item->key; }
	};

	TEST_CLASS(HashTest)
	{
	private:
		using ItemHash = Hash<Item, String, KeyExtractor>;
		using ItemPtrHash = Hash<const Item*, String, PointerKeyExtractor>;
		using StringHash = Hash<String>;
		using StringArray = DArray<String>;

		static const size_t ITEMS_COUNT = 10;
		static Item testItems[ITEMS_COUNT];

		static bool areAllTestItemsIn(const ItemHash& hash)
		{
			return contains(hash, testItems, KeyExtractor{});
		}

		static bool contains(const StringHash& hash, StringArray values)
		{			
			return contains(hash, values, IDragnev::Identity{});
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun = std::hash<Key>, typename Container>
		static bool contains(const Hash<Item, Key, KeyAccessor, HashFun>& hash, const Container& items, KeyAccessor keyOf)
		{
			for (auto&& item : items)
			{
				if (auto found = hash.search(keyOf(item));
					!found || found.value() != item)
				{
					return false;
				}
			}

			return true;
		}

	public:
		TEST_METHOD(defaultCtor)
		{
			ItemHash hash{};

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(expectedCountCtorMakesEmptyHash)
		{
			ItemHash hash{ ITEMS_COUNT };

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(rangeCtor)
		{
			ItemHash hash{ begin(testItems), end(testItems) };

			Assert::IsTrue(areAllTestItemsIn(hash));
		}

		TEST_METHOD(rangeCtorFromMoveIterator)
		{
			auto names = StringArray{ "One", "Two", "Three" };
			auto first = std::make_move_iterator(begin(names));
			auto last = std::make_move_iterator(end(names));

			StringHash hash{ first, last };

			Assert::IsTrue(names == StringArray{ "", "", "" }, L"The values are not moved");
			Assert::IsTrue(contains(hash, { "One", "Two", "Three" }));
		}

		TEST_METHOD(initListCtor)
		{
			StringHash hash{ "One", "Two", "Three" };

			Assert::IsTrue(contains(hash, { "One", "Two", "Three" }));
		}
		TEST_METHOD(insertUpdatesCount)
		{
			ItemHash hash{ ITEMS_COUNT };

			hash.insert(Item{ "Key" });
			Assert::AreEqual(hash.getCount(), 1U);
		}

		TEST_METHOD(insertingRValue)
		{
			Hash<String> hash;
			auto str = String{ "Key" };

			hash.insert(std::move(str));

			Assert::IsTrue(str == String{ "" }, L"The rvalue is not moved");
			Assert::AreEqual(hash.getCount(), 1U, L"Count is not updated");
		}
	
		TEST_METHOD(removingUniqueItem)
		{
			ItemHash hash{ begin(testItems), end(testItems) };
			auto& key = testItems[0].key;

			hash.remove(key);
			auto found = hash.search(key);

			Assert::IsTrue(hash.getCount() == ITEMS_COUNT - 1, L"Count is not updated");
			Assert::IsFalse(found.has_value(), L"Item is found after remove");
		}

		TEST_METHOD(removingDuplicateItem)
		{
			auto duplicate = String{ "duplicate" };
			StringHash hash{ duplicate, "unique", duplicate };

			hash.remove(duplicate);
			auto found = hash.search(duplicate);

			Assert::IsTrue(found.has_value(), L"Null optional was found");
			Assert::IsTrue(found.value() == duplicate, L"Wrong item was found");
		}
		TEST_METHOD(empty)
		{
			ItemHash hash{ begin(testItems), end(testItems) };

			hash.empty();

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(searchingAnInsertedItem)
		{
			ItemHash hash{ begin(testItems), end(testItems) };
			auto& expected = testItems[0];
			auto found = hash.search(expected.key);

			Assert::IsTrue(found.has_value(), L"Search returns an empty optional");
			Assert::IsTrue(found.value() == expected, L"Search returns invalid item");
		}
		TEST_METHOD(searchingAMissingItem)
		{
			ItemHash hash{};

			for (auto&& item : testItems)
			{
				auto found = hash.search(item.key);
				Assert::IsFalse(found.has_value());
			}
		}

		TEST_METHOD(searchingAPointerHashReturnsAPointer)
		{
			ItemPtrHash hash{ &testItems[0], &testItems[1], &testItems[2] };
			auto expected = &testItems[0];

			auto found = hash.search(expected->key);

			Assert::IsNotNull(found, L"Search returns a null pointer");
			Assert::IsTrue(found == expected, L"Search returns a wrong pointer");
		}

		TEST_METHOD(searchingAMissingPointerOnAPointerHashReturnsNullptr)
		{
			ItemPtrHash hash{};

			for (auto&& item : testItems)
			{
				auto found = hash.search(item.key);
				Assert::IsNull(found);
			}
		}

		TEST_METHOD(moveCtorFromEmptyHash)
		{
			ItemHash source{ ITEMS_COUNT };
			ItemHash hash{ std::move(source) };

			Assert::IsTrue(hash.isEmpty(), L"Moved-in hash is not empty");
			Assert::IsTrue(source.isEmpty(), L"Moved-from hash is not empty");
		}

		TEST_METHOD(moveCtorFromNonEmpty)
		{
			ItemHash source{ begin(testItems), end(testItems) };
			ItemHash destination{ std::move(source) };

			Assert::IsTrue(source.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Moved-in hash has invalid content");
		}

		TEST_METHOD(copyAssignmentFromEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(copyAssignmentFromEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(copyAssignmentFromNonEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs = rhs;

			Assert::IsTrue(areAllTestItemsIn(lhs));
		}

		TEST_METHOD(copyAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs.remove(testItems[0].key);
			lhs.remove(testItems[1].key);

			lhs = rhs;

			Assert::IsTrue(areAllTestItemsIn(lhs));
		}

		TEST_METHOD(moveAssignmentFromEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-into hash is not empty");
		}

		TEST_METHOD(moveAssignmentFromEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-into hash is not empty");
		}

		TEST_METHOD(moveAssignmentFromNonEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Moved-in hash has invalid content");
		}

		TEST_METHOD(moveAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs.remove(testItems[0].key);
			lhs.remove(testItems[1].key);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Moved-in hash has invalid content");
		}

		TEST_METHOD(copyCtorFromEmpty)
		{
			ItemHash source{ ITEMS_COUNT };
			ItemHash destination{ source };

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(copyCtorFromNonEmpty)
		{
			ItemHash source{ begin(testItems), end(testItems) };
			ItemHash destination(source);

			Assert::IsTrue(areAllTestItemsIn(destination));
		}

	};

	Item HashTest::testItems[10]{ { "some key" }, { "other key" }, { "keeeey" }, { "New York" }, { "Lambda" }, { "No idea" }, { "Creative" }, { "Sofia" }, { "123" }, { "Last" } };
}