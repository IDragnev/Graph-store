#include "CppUnitTest.h"
#include "../../Graph store/String/String.h"
#include "../../Graph store/Hash/Hash.h"
#include "../../Graph store/Hash/HashFunctionStringSpecialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HashTest
{		
	class Item
	{
	public:
		Item(const char* key = "str") : key(key) {}

		bool operator==(const Item& other) const { return key == other.key; }

		const String& getKey() const { return key; }
		void setKey(const char* key) { this->key = key; }

	private:
		String key;
	};

	class KeyExtractor
	{
	public:
		const String& operator()(const Item& item)const
		{
			return item.getKey();
		}
	};

	TEST_CLASS(HashTest)
	{
	private:
		typedef Hash<Item, String, KeyExtractor> ItemHash;

		static DArray<Item> testItems;
		static const size_t EXPECTED_COUNT = 10;
		static const size_t TEST_ITEMS_COUNT = EXPECTED_COUNT;

		static bool areAllTestItemsIn(const ItemHash& hash)
		{
			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				if (hash.search(testItems[i].getKey()) != &testItems[i])
				{
					return false;
				}
			}

			return true;
		}

		static void insertTestItemsTo(ItemHash& hash)
		{
			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				hash.insert(testItems[i]);
			}
		}

		static void initializeTestItems()
		{
			static const size_t keysCount = 15;
			static const char* keys[] = { "some key" , "other key", "@asd" , "not very creative", "New York"
				"another key", "key 5", "123" , "123123", "running out of ideas",
				"Sofia", "IBN", "Word", "Testing", "Tired" };

			for (size_t i = 0; i < TEST_ITEMS_COUNT && i < keysCount; ++i)
			{
				testItems[i].setKey(keys[i]);
			}
		}

	public:
		TEST_CLASS_INITIALIZE(itemsInitialization)
		{
			initializeTestItems();
		}

		TEST_METHOD(testConstructorMakesEmptyHash)
		{
			ItemHash hash(EXPECTED_COUNT);

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testInsertUpdatesCount)
		{
			ItemHash hash(EXPECTED_COUNT);

			for (size_t i = 1; i < TEST_ITEMS_COUNT; ++i)
			{
				hash.insert(testItems[i]);

				Assert::AreEqual(hash.getCount(), i);
			}
		}

		TEST_METHOD(testEmptyLeavesTheHashEmpty)
		{
			ItemHash hash(EXPECTED_COUNT);
			insertTestItemsTo(hash);

			hash.empty();

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testRemoveReturnsCorrectAddress)
		{
			ItemHash hash(EXPECTED_COUNT);
			insertTestItemsTo(hash);

			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				Assert::AreEqual(hash.getCount(), TEST_ITEMS_COUNT - i, L"Remove does not update count");
				Assert::IsTrue(hash.remove(testItems[i].getKey()) == &testItems[i], L"Remove is not returning the correct address");
			}
		}

		TEST_METHOD(testRemoveReturnsNullptrOnEmptyHash)
		{
			ItemHash hash(EXPECTED_COUNT);

			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				Assert::IsNull(hash.remove(testItems[i].getKey()));
			}
		}

		TEST_METHOD(testSearchReturnsCorrectAddress)
		{
			ItemHash hash(EXPECTED_COUNT);
			insertTestItemsTo(hash);

			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				Assert::IsTrue(hash.search(testItems[i].getKey()) == &testItems[i]);
			}
		}

		TEST_METHOD(testSearchOnEmptyHashReturnsNullptr)
		{
			ItemHash hash(EXPECTED_COUNT);

			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				Assert::IsNull(hash.search(testItems[i].getKey()));
			}
		}

		TEST_METHOD(testMoveCtorFromEmptyHash)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash hash(std::move(source));

			Assert::IsTrue(hash.isEmpty(), L"Move constructed object with an empty hash is not empty");
			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
		}

		TEST_METHOD(testMoveCtorFromNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			insertTestItemsTo(source);

			ItemHash destination(std::move(source));

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::AreEqual(destination.getCount(), testItems.getCount(), L"Move construction is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Move construction is not moving items properly");
		}

		TEST_METHOD(testCopyAssignmentFromEmptyToEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty(), L"Lhs is not empty after assigning it an empty rhs");
		}

		TEST_METHOD(testCopyAssignmentFromEmptyToNonEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);			
			insertTestItemsTo(lhs);

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty(), L"Lhs is not empty after assigning it an empty rhs");
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);
			insertTestItemsTo(rhs);

			lhs = rhs;

			Assert::AreEqual(lhs.getCount(), rhs.getCount(), L"Copy assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);

			insertTestItemsTo(lhs);
			insertTestItemsTo(rhs);			
			lhs.remove(testItems[0].getKey());
			lhs.remove(testItems[1].getKey());

			lhs = rhs;

			Assert::AreEqual(lhs.getCount(), rhs.getCount(), L"Copy assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyToEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-into object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyToNonEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);
			insertTestItemsTo(lhs);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-into object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyToEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);
			insertTestItemsTo(rhs);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::AreEqual(lhs.getCount(), testItems.getCount(), L"Move assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Move assignment is not moving the table properly");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash lhs(EXPECTED_COUNT);
			ItemHash rhs(EXPECTED_COUNT);

			insertTestItemsTo(lhs);
			insertTestItemsTo(rhs);
			lhs.remove(testItems[0].getKey());
			lhs.remove(testItems[1].getKey());

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::AreEqual(lhs.getCount(), testItems.getCount(), L"Move assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(lhs), L"Move assignment is not moving the table properly");
		}

		TEST_METHOD(testCopyCtorFromEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(source);

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(testCopyCtorFromNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			insertTestItemsTo(source);

			ItemHash destination(source);

			Assert::AreEqual(destination.getCount(), source.getCount(), L"Copy construction is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Copy-construction is not copying item tables properly");
		}
		
	};

	DArray<Item> HashTest::testItems(TEST_ITEMS_COUNT, TEST_ITEMS_COUNT);

}