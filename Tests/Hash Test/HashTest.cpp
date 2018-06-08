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

		bool operator==(const Item& other)const { return key == other.key; }

		const String& getKey()const { return key; }
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

	typedef Hash<Item, String, KeyExtractor> ItemHash;

	TEST_CLASS(HashTest)
	{
	private:
		static DArray<Item> testItems;
		static const size_t EXPECTED_COUNT = 10;

		static bool areAllTestItemsIn(const ItemHash& hash)
		{
			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
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
			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
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

			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount && i < keysCount; ++i)
			{
				testItems[i].setKey(keys[i]);
			}
		}

	public:
		TEST_CLASS_INITIALIZE(ItemsInitialization)
		{
			initializeTestItems();
		}

		TEST_METHOD(testConstructorMakesEmptyHash)
		{
			ItemHash hash(EXPECTED_COUNT);
			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testInsertionUpdatesCount)
		{
			ItemHash hash(EXPECTED_COUNT);

			const size_t itemsCount = testItems.getCount();

			for (size_t i = 1; i < itemsCount; ++i)
			{
				hash.insert(testItems[i]);
				Assert::IsTrue(hash.getCount() == i);
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

			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.getCount() == itemsCount - i, L"Remove does not update count");
				Assert::IsTrue(hash.remove(testItems[i].getKey()) == &testItems[i], L"Remove is not returning the correct address");
			}
		}

		TEST_METHOD(testRemoveReturnsNullptrOnEmptyHash)
		{
			ItemHash hash(EXPECTED_COUNT);

			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.remove(testItems[i].getKey()) == nullptr);
			}
		}

		TEST_METHOD(testSearchReturnsCorrectAddress)
		{
			ItemHash hash(EXPECTED_COUNT);

			insertTestItemsTo(hash);

			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.search(testItems[i].getKey()) == &testItems[i]);
			}
		}

		TEST_METHOD(testSearchOnEmptyHashReturnsNullptr)
		{
			ItemHash hash(EXPECTED_COUNT);

			const size_t itemsCount = testItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.search(testItems[i].getKey()) == nullptr);
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

			Assert::IsTrue(destination.getCount()  == testItems.getCount(), L"Move construction is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Move construction is not moving items properly");
		}

		TEST_METHOD(testCopyAssignmentFromEmptyToEmptyTest)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			destination = source;

			Assert::IsTrue(destination.isEmpty());
			Assert::IsTrue(source.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentFromEmptyToNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);
			
			insertTestItemsTo(destination);
			destination = source;

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertTestItemsTo(source);

			destination = source;

			Assert::AreEqual(destination.getCount(), source.getCount(), L"Copy assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertTestItemsTo(source);
			insertTestItemsTo(destination);
			
			destination.remove(testItems[0].getKey());
			destination.remove(testItems[1].getKey());

			destination = source;

			Assert::AreEqual(destination.getCount(), source.getCount(), L"Copy assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyToEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			destination = std::move(source);

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::IsTrue(destination.isEmpty(), L"Moved-into object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyToNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertTestItemsTo(destination);

			destination = std::move(source);

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::IsTrue(destination.isEmpty(), L"Moved-into object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyToEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertTestItemsTo(source);

			destination = std::move(source);

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::AreEqual(destination.getCount(), testItems.getCount(), L"Move assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Move assignment is not moving the table properly");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertTestItemsTo(source);
			insertTestItemsTo(destination);

			destination.remove(testItems[0].getKey());
			destination.remove(testItems[1].getKey());

			destination = std::move(source);

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::AreEqual(destination.getCount(), testItems.getCount(), L"Move assignment is not handling count properly");
			Assert::IsTrue(areAllTestItemsIn(destination), L"Move assignment is not moving the table properly");
		}

		TEST_METHOD(testCopyCtorFromEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash hash(source);

			Assert::IsTrue(hash.isEmpty());
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

	DArray<Item> HashTest::testItems(HashTest::EXPECTED_COUNT, HashTest::EXPECTED_COUNT);

}