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
				Assert::IsTrue(hash.isEmpty(), L"isEmpty returns false directy after construction");
				Assert::IsTrue(hash.getCount() == 0, L"Count is not zero before inserting any object");
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

			hash.empty();

			Assert::IsTrue(hash.isEmpty(), L"isEmpty() returns false after calling empty()");
			Assert::IsTrue(hash.getCount() == 0, L"Count is not 0 after calling empty()");
		}


		TEST_METHOD(RemovalTest)
		{
			ItemHash hash(10);

			insertItemsToHash(hash, testClassItems);

			const int ITEMS_COUNT = testClassItems.getCount();

			for (int i = 0; i < ITEMS_COUNT; ++i)
			{
				Assert::IsTrue(hash.getCount() == ITEMS_COUNT - i, L"Remove does not update count");
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
				Assert::IsTrue(hash.getCount() == ITEMS_COUNT, L"Search modifies count");
			}

			hash.empty();

			for (int i = 0; i < ITEMS_COUNT; ++i)
				Assert::IsTrue(hash.search(testClassItems[i].getKey()) == nullptr, L"Search returns non-null address after calling empty");
		}


		TEST_METHOD(MoveConstructorTest)
		{
			ItemHash source(10);
			ItemHash hash(std::move(source));

			Assert::IsTrue(hash.isEmpty(), L"Move constructed object with an empty hash is not empty");
			Assert::IsTrue(hash.getCount() == 0, L"Count is not zero after move-construction from an empty object");
			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::IsTrue(source.getCount() == 0, L"Moved object's count is not zero");

			insertItemsToHash(source, testClassItems);

			ItemHash destination(std::move(source));

			Assert::IsTrue(source.isEmpty(), L"Moved object is not empty");
			Assert::IsTrue(source.getCount() == 0, L"Moved object's count is not zero");

			const int ITEMS_COUNT = testClassItems.getCount();

			Assert::IsTrue(destination.getCount() == ITEMS_COUNT, L"Move construction is not handling count properly");

			for (int i = 0; i < ITEMS_COUNT; ++i)
				Assert::IsTrue(destination.search(testClassItems[i].getKey()) == &testClassItems[i], L"Move construction is not moving items properly");
		}
	};

	DArray<Item> HashTest::testClassItems(15, 10);

}