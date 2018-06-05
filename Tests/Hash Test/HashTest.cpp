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


	void setItemsWithKeys(DArray<Item>& items)
	{
		static const size_t keysCount = 15;
		static const char* keys[] = { "some key" , "other key", "@asd" , "not very creative", "New York"
								      "another key", "key 5", "123" , "123123", "running out of ideas", 
									  "Sofia", "IBN", "Word", "Testing", "Tired" };

		const size_t ITEMS_COUNT = items.getCount();

		for (size_t i = 0; i < ITEMS_COUNT && i < keysCount; ++i)
			items[i].setKey(keys[i]);
	}


	void insertItemsToHash(ItemHash& hash, DArray<Item>& items)
	{
		const size_t ITEMS_COUNT = items.getCount();

		for (size_t i = 0; i < ITEMS_COUNT; ++i)
			hash.insert(items[i]);
	}


	TEST_CLASS(HashTest)
	{
	private:
		static DArray<Item> testClassItems;
		static const size_t EXPECTED_COUNT = 10;

		static void testAreAllTestItemsInAHash(ItemHash& hash, const wchar_t* message)
		{
			const size_t itemsCount = testClassItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == &testClassItems[i], message);
		}

	public:
		TEST_CLASS_INITIALIZE(ItemsInitialization)
		{
			setItemsWithKeys(testClassItems);
		}

		TEST_METHOD(testConstructorMakesEmptyHash)
		{
			for (size_t i = 1; i < 50; ++i)
			{
				ItemHash hash(i);
				Assert::IsTrue(hash.isEmpty());
			}
		}

		TEST_METHOD(testInsertionUpdatesCount)
		{
			ItemHash hash(EXPECTED_COUNT);

			const size_t itemsCount = testClassItems.getCount();

			for (size_t i = 1; i < itemsCount; ++i)
			{
				hash.insert(testClassItems[i]);
				Assert::IsTrue(hash.getCount() == i);
			}
		}

		TEST_METHOD(testEmptyLeavesTheHashEmpty)
		{
			ItemHash hash(EXPECTED_COUNT);

			insertItemsToHash(hash, testClassItems);
			hash.empty();

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testRemoveReturnsCorrectAddress)
		{
			ItemHash hash(EXPECTED_COUNT);

			insertItemsToHash(hash, testClassItems);

			const size_t itemsCount = testClassItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.getCount() == itemsCount - i, L"Remove does not update count");
				Assert::IsTrue(hash.remove(testClassItems[i].getKey()) == &testClassItems[i], L"Remove is not returning the correct address");
			}
		}

		TEST_METHOD(testRemoveReturnsNullptrOnEmptyHash)
		{
			ItemHash hash(EXPECTED_COUNT);

			const size_t itemsCount = testClassItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.remove(testClassItems[i].getKey()) == nullptr);
			}
		}

		TEST_METHOD(testSearchReturnsCorrectAddress)
		{
			ItemHash hash(EXPECTED_COUNT);

			insertItemsToHash(hash, testClassItems);

			const size_t itemsCount = testClassItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == &testClassItems[i]);
			}
		}

		TEST_METHOD(testSearchOnEmptyHashReturnsNullptr)
		{
			ItemHash hash(EXPECTED_COUNT);

			const size_t itemsCount = testClassItems.getCount();

			for (size_t i = 0; i < itemsCount; ++i)
			{
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == nullptr);
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
			ItemHash destination(EXPECTED_COUNT);

			insertItemsToHash(source, testClassItems);

			ItemHash destination(std::move(source));

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");

			Assert::IsTrue(destination.getCount()  == testClassItems.getCount(), L"Move construction is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Move construction is not moving items properly");
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
			
			insertItemsToHash(destination, testClassItems);
			destination = source;

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertItemsToHash(source, testClassItems);

			destination = source;

			Assert::AreEqual(destination.getCount(), source.getCount(), L"Copy assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyToNonEmpty)
		{
			ItemHash source(EXPECTED_COUNT);
			ItemHash destination(EXPECTED_COUNT);

			insertItemsToHash(source, testClassItems);
			insertItemsToHash(destination, testClassItems);
			
			destination.remove(testClassItems[0].getKey());
			destination.remove(testClassItems[1].getKey());

			destination = source;

			Assert::AreEqual(destination.getCount(), source.getCount(), L"Copy assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(MoveAssignmentFromEmptyTest)
		{
			ItemHash source(10);
			ItemHash destination(5);

			//empty to empty
			destination = std::move(source);

			assertHashIsEmpty(destination, L"LHS is not empty after move assignment from an empty object");
			assertHashCountEqualsValue(destination, 0, L"LHS' count is not zero after move assignment from an empty object");

			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertHashCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");

			//empty to non-empty
			insertItemsToHash(destination, testClassItems);

			destination = std::move(source);

			assertHashIsEmpty(destination, L"LHS is not empty after move assignment from an empty object");
			assertHashCountEqualsValue(destination, 0, L"LHS' count is not zero after move assignment from an empty object");
			
			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertHashCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");

		}

		TEST_METHOD(MoveAssignmentFromNonEmptyTest)
		{
			ItemHash source(10);
			ItemHash destination(5);

			//non empty to empty
			insertItemsToHash(source, testClassItems);

			destination = std::move(source);

			assertHashCountEqualsValue(destination, testClassItems.getCount(), L"Move assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Move assignment is not moving the table properly");

			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertHashCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");

			//non empty to non empty
			insertItemsToHash(source, testClassItems);

			destination.remove(testClassItems[0].getKey());
			destination.remove(testClassItems[1].getKey());

			destination = std::move(source);

			assertHashCountEqualsValue(destination, testClassItems.getCount(), L"Move assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Move assignment is not moving the table properly");

			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertHashCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");
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
			insertItemsToHash(source, testClassItems);

			ItemHash destination(source);

			Assert::AreEqual(destination.getCount(), source.getCount(), L"Copy construction is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Copy-construction is not copying item tables properly");
		}
		
	};

	DArray<Item> HashTest::testClassItems(15, 10);

}