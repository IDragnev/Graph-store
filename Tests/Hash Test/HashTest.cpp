#include "CppUnitTest.h"
#include "../../Graph store/String/String.h"
#include "../../Graph store/Containers/Hash/Hash.h"
#include "../../Graph store/Containers/Hash/HashFunctionStringSpecialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::begin;
using std::end;

namespace HashTest
{
	struct Item
	{
		Item(const String& str) : key{ str } {}
		String key{};
	};

	bool operator==(const Item& lhs, const Item& rhs) { return lhs.key == rhs.key; }

	struct KeyExtractor
	{
		const String& operator()(const Item& item) const { return item.key; }
	};

	TEST_CLASS(HashTest)
	{
	private:
		using ItemHash = Containers::Hash<Item, String, KeyExtractor>;

		static const size_t ITEMS_COUNT = 10;
		static Item testItems[ITEMS_COUNT];

		static bool areAllTestItemsIn(const ItemHash& hash)
		{
			for (auto&& item : testItems)
			{
				if (hash.search(item.key) != &item)
				{
					return false;
				}
			}

			return true;
		}

	public:
		TEST_METHOD(testDefaultCtor)
		{
			ItemHash hash{};

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testConstructorMakesEmptyHash)
		{
			ItemHash hash{ ITEMS_COUNT };

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testRangeCtor)
		{
			ItemHash hash{ begin(testItems), end(testItems) };

			Assert::IsTrue(areAllTestItemsIn(hash));
		}

		TEST_METHOD(testInsertUpdatesCount)
		{
			ItemHash hash{ ITEMS_COUNT };

			auto i = 1U;
			for (auto&& item : testItems)
			{
				hash.insert(item);
				Assert::AreEqual(hash.getCount(), i++);
			}
		}

		TEST_METHOD(testEmptyLeavesTheHashEmpty)
		{
			ItemHash hash{ begin(testItems), end(testItems) };

			hash.empty();

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testRemoveReturnsCorrectAddress)
		{
			ItemHash hash{ begin(testItems), end(testItems) };

			auto count = ITEMS_COUNT;
			for (auto&& item : testItems)
			{
				Assert::IsTrue(hash.remove(item.key) == &item, L"Invalid address");
				Assert::AreEqual(hash.getCount(), --count, L"Count is not updated");
			}
		}

		TEST_METHOD(testRemoveReturnsNullptrOnEmptyHash)
		{
			ItemHash hash{ ITEMS_COUNT };

			for (auto&& item : testItems)
			{
				Assert::IsNull(hash.remove(item.key));
			}
		}

		TEST_METHOD(testSearchReturnsCorrectAddress)
		{
			ItemHash hash{ begin(testItems), end(testItems) };

			for (auto&& item : testItems)
			{
				Assert::IsTrue(hash.search(item.key) == &item);
			}
		}

		TEST_METHOD(testSearchOnEmptyHashReturnsNullptr)
		{
			ItemHash hash{ ITEMS_COUNT };

			for (auto&& item : testItems)
			{
				Assert::IsNull(hash.search(item.key));
			}
		}

		TEST_METHOD(testMoveCtorFromEmptyHash)
		{
			ItemHash source{ ITEMS_COUNT };
			ItemHash hash{ std::move(source) };

			Assert::IsTrue(hash.isEmpty(), L"Moved-in hash is not empty");
			Assert::IsTrue(source.isEmpty(), L"Moved-from hash is not empty");
		}

		TEST_METHOD(testMoveCtorFromNonEmpty)
		{
			ItemHash source{ begin(testItems), end(testItems) };
			ItemHash destination{ std::move(source) };

			Assert::IsTrue(source.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Moved-in hash has invalid content");
		}

		TEST_METHOD(testCopyAssignmentFromEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentFromEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs = rhs;

			Assert::IsTrue(areAllTestItemsIn(lhs));
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs.remove(testItems[0].key);
			lhs.remove(testItems[1].key);

			lhs = rhs;

			Assert::IsTrue(areAllTestItemsIn(lhs));
		}

		TEST_METHOD(testMoveAssignmentFromEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-into hash is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ ITEMS_COUNT };

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-into hash is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyToEmpty)
		{
			ItemHash lhs{ ITEMS_COUNT };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Moved-in hash has invalid content");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash lhs{ begin(testItems), end(testItems) };
			ItemHash rhs{ begin(testItems), end(testItems) };

			lhs.remove(testItems[0].key);
			lhs.remove(testItems[1].key);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from hash is not empty");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Moved-in hash has invalid content");
		}

		TEST_METHOD(testCopyCtorFromEmpty)
		{
			ItemHash source{ ITEMS_COUNT };
			ItemHash destination{ source };

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(testCopyCtorFromNonEmpty)
		{
			ItemHash source{ begin(testItems), end(testItems) };
			ItemHash destination(source);

			Assert::IsTrue(areAllTestItemsIn(destination));
		}

	};

	Item HashTest::testItems[10]{ { "some key" }, { "other key" }, { "keeeey" }, { "New York" }, { "Lambda" }, { "No idea" }, { "Creative" }, { "Sofia" }, { "123" }, { "Last" } };
}