#include "CppUnitTest.h"
#include "../Graph store/String/String.h"
#include "../Graph store/Hash/Hash.h"
#include "../Graph store/Hash/HashFunctionStringSpecialization.h"

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
		static const int keysCount = 15;
		static const char* keys[] = { "some key" , "other key", "@asd" , "not very creative", "New York"
								      "another key", "key 5", "123" , "123123", "running out of ideas", 
									  "Sofia", "IBN", "Word", "Testing", "Tired" };

		const int ITEMS_COUNT = items.getCount();

		for (int i = 0; i < ITEMS_COUNT && i < keysCount; ++i)
			items[i].setKey(keys[i]);
	}


	void insertItemsToHash(ItemHash& hash, DArray<Item>& items)
	{
		const int ITEMS_COUNT = items.getCount();

		for (int i = 0; i < ITEMS_COUNT; ++i)
			hash.insert(items[i]);
	}


	TEST_CLASS(HashTest)
	{
	private:
		static DArray<Item> testClassItems;

		void testAreAllTestItemsInAHash(ItemHash& hash, const wchar_t* message)
		{
			const int ITEMS_COUNT = testClassItems.getCount();

			for (int i = 0; i < ITEMS_COUNT; ++i)
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == &testClassItems[i], message);
		}

		void assertCountEqualsValue(const ItemHash& hash, int value, const wchar_t* message)
		{
			Assert::IsTrue(hash.getCount() == value, message);
		}

		void assertHashIsEmpty(const ItemHash& hash, const wchar_t* message)
		{
			Assert::IsTrue(hash.isEmpty(), message);
		}


	public:
		TEST_CLASS_INITIALIZE(ItemsInitialization)
		{
			setItemsWithKeys(testClassItems);
		}


		TEST_METHOD(ConstructorTest)
		{
			for (int i = 1; i < 50; ++i)
			{
				ItemHash hash(i);
				assertHashIsEmpty(hash, L"isEmpty returns false directy after construction");
				assertCountEqualsValue(hash, 0, L"Count is not zero before inserting any object");
			}
		}


		TEST_METHOD(InsertionTest)
		{
			ItemHash hash(10);

			const int ITEMS_COUNT = testClassItems.getCount();

			for (int i = 0; i < ITEMS_COUNT; ++i)
			{
				Assert::IsTrue(hash.getCount() == i, L"Insertion does not update count");
				hash.insert(testClassItems[i]);
				Assert::IsFalse(hash.isEmpty(), L"isEmpty returns true after insertion");
			}
		}

		TEST_METHOD(EmptyingHashTest)
		{
			ItemHash hash(10);

			insertItemsToHash(hash, testClassItems);

			hash.empty();

			assertHashIsEmpty(hash, L"isEmpty() returns false after calling empty()");
			assertCountEqualsValue(hash, 0, L"Count is not 0 after calling empty()");
		}

		TEST_METHOD(RemovalTest)
		{
			ItemHash hash(10);

			insertItemsToHash(hash, testClassItems);

			const int ITEMS_COUNT = testClassItems.getCount();

			for (int i = 0; i < ITEMS_COUNT; ++i)
			{
				assertCountEqualsValue(hash, ITEMS_COUNT - i, L"Remove does not update count");
				Assert::IsTrue(hash.remove(testClassItems[i].getKey()) == &testClassItems[i], L"Remove is not returning the correct address");
			}

			hash.empty();

			for (int i = 0; i < ITEMS_COUNT; ++i)
				Assert::IsTrue(hash.remove(testClassItems[i].getKey()) == nullptr, L"Remove returns non-null address after calling empty");
		}


		TEST_METHOD(SearchingTest)
		{
			ItemHash hash(10);

			insertItemsToHash(hash, testClassItems);

			const int ITEMS_COUNT = testClassItems.getCount();

			for (int i = 0; i < ITEMS_COUNT; ++i)
			{
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == &testClassItems[i], L"Search is not returning the correct address");
				assertCountEqualsValue(hash, ITEMS_COUNT, L"Search modifies count");
			}

			hash.empty();

			for (int i = 0; i < ITEMS_COUNT; ++i)
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == nullptr, L"Search returns non-null address after calling empty");
		}


		TEST_METHOD(MoveConstructorTest)
		{
			ItemHash source(10);
			ItemHash hash(std::move(source));

			assertHashIsEmpty(hash, L"Move constructed object with an empty hash is not empty");
			assertCountEqualsValue(hash, 0, L"Count is not zero after move-construction from an empty object");

			assertHashIsEmpty(source, L"Moved object is not empty");
			assertCountEqualsValue(source, 0, L"Moved object's count is not zero");

			insertItemsToHash(source, testClassItems);

			ItemHash destination(std::move(source));

			assertHashIsEmpty(source, L"Moved object is not empty");
			assertCountEqualsValue(source, 0, L"Moved object's count is not zero");

			assertCountEqualsValue(destination, testClassItems.getCount(), L"Move construction is not handling count properly");

			testAreAllTestItemsInAHash(destination, L"Move construction is not moving items properly");
		}

		TEST_METHOD(CopyAssignmentFromEmptyTest)
		{
			ItemHash source(10);
			ItemHash destination(5);

			//empty to empty
			destination = source;

			assertHashIsEmpty(destination, L"LHS is not empty after copy assignment from an empty object");
			assertCountEqualsValue(destination, 0, L"LHS' count is not zero after copy assignment from an empty object");

			//empty to non empty
			insertItemsToHash(destination, testClassItems);

			destination = source;

			assertHashIsEmpty(destination, L"LHS is not empty after copy assignment from an empty object");
			assertCountEqualsValue(destination, 0, L"LHS' count is not zero after copy assignment from an empty object");

		}

		TEST_METHOD(CopyAssignmentFromNonEmptyTest)
		{
			ItemHash source(10);
			ItemHash destination(5);

			insertItemsToHash(source, testClassItems);
			
			//non empty to empty
			destination = source;

			assertCountEqualsValue(destination, source.getCount(), L"Copy assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Copy assignment is not copying tables properly");

			//non empty to non empty
			destination.remove(testClassItems[0].getKey());
			destination.remove(testClassItems[1].getKey());

			destination = source;

			assertCountEqualsValue(destination, source.getCount(), L"Copy assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Copy assignment is not copying tables properly");
		}

		TEST_METHOD(MoveAssignmentFromEmptyTest)
		{
			ItemHash source(10);
			ItemHash destination(5);

			//empty to empty
			destination = std::move(source);

			assertHashIsEmpty(destination, L"LHS is not empty after move assignment from an empty object");
			assertCountEqualsValue(destination, 0, L"LHS' count is not zero after move assignment from an empty object");

			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");

			//empty to non-empty
			insertItemsToHash(destination, testClassItems);

			destination = std::move(source);

			assertHashIsEmpty(destination, L"LHS is not empty after move assignment from an empty object");
			assertCountEqualsValue(destination, 0, L"LHS' count is not zero after move assignment from an empty object");
			
			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");

		}

		TEST_METHOD(MoveAssignmentFromNonEmptyTest)
		{
			ItemHash source(10);
			ItemHash destination(5);

			//non empty to empty
			insertItemsToHash(source, testClassItems);

			destination = std::move(source);

			assertCountEqualsValue(destination, testClassItems.getCount(), L"Move assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Move assignment is not moving the table properly");

			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");

			//non empty to non empty
			insertItemsToHash(source, testClassItems);

			destination.remove(testClassItems[0].getKey());
			destination.remove(testClassItems[1].getKey());

			destination = std::move(source);

			assertCountEqualsValue(destination, testClassItems.getCount(), L"Move assignment is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Move assignment is not moving the table properly");

			assertHashIsEmpty(source, L"RHS is not empty after move assignment");
			assertCountEqualsValue(source, 0, L"RHS' count is not zero after move assignment");
		}

		TEST_METHOD(CopyConstructorTest)
		{
			//from empty
			ItemHash source(10);
			ItemHash hash(source);

			assertHashIsEmpty(hash, L"Copy-constructed object with an empty hash is not empty");
			assertCountEqualsValue(hash, 0, L"Count is not zero after copy-construction from an empty object");
			
			//from non empty
			insertItemsToHash(source, testClassItems);

			ItemHash destination(source);

			assertCountEqualsValue(destination, source.getCount(), L"Copy construction is not handling count properly");
			testAreAllTestItemsInAHash(destination, L"Copy-construction is not copying item tables properly");
		}
		
	};

	DArray<Item> HashTest::testClassItems(15, 10);

}