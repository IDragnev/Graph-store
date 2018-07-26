#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/Priority Queue/PriorityQueue.h"

#include "TestItem.h"
#include "HandleSetter.h"
#include "CompareFunction.h"

namespace PriorityQueueTest
{	
	TEST_CLASS(PriorityQueueTest)
	{
	private:
		typedef PriorityQueue<TestItem, unsigned, CompareFunction, HandleSetter> MaxPriorityQueue;
		typedef DArray<TestItem> TestItemArray;
		typedef TestItemArray::Iterator ItemIterator;
		typedef Pair<TestItem, unsigned> TestPair;

		static TestItemArray testItems;
		static const size_t TEST_ITEMS_COUNT = 8;

	private:
		static TestPair makePair(TestItem& item, unsigned key)
		{
			return TestPair(&item, key);
		}

		static void insertTestItemsByIDTo(MaxPriorityQueue& queue)
		{
			ItemIterator iterator = testItems.getHeadIterator();

			while (iterator)
			{
				TestItem& currentItem = *iterator;
				queue.insert(makePair(currentItem, currentItem.getID()));

				++iterator;
			}
		}

		static unsigned getGreatestItemID()
		{
			return testItems[TEST_ITEMS_COUNT - 1].getID();
		}

	public:	
		TEST_CLASS_INITIALIZE(initializeItemsWithIncreasingIDs)
		{
			for (size_t i = 0; i < TEST_ITEMS_COUNT; ++i)
			{
				testItems[i].setID(i);
			}
		}

		TEST_METHOD(testDefaultConstructedQueueIsEmpty)
		{
			MaxPriorityQueue queue;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testInsertArrangesTheItemsCorrectly)
		{
			MaxPriorityQueue queue;
			insertTestItemsByIDTo(queue);
			
			unsigned greatestID = getGreatestItemID();
			const TestItem& optimal = queue.getOptimal();

			Assert::AreEqual(optimal.getID(), greatestID);
		}

		TEST_METHOD(testImproveKeyRearangesInsertedItems)
		{
			MaxPriorityQueue queue;
			TestItem& expectedItem = testItems[0];

			queue.insert(makePair(expectedItem, 0));
			queue.insert(makePair(testItems[1], 1));

			queue.improveKey(expectedItem.getHandle(), 2);
			const TestItem& optimal = queue.getOptimal();

			Assert::IsTrue(expectedItem == optimal);
		}

		TEST_METHOD(testExtractOptimalWithJustOneItem)
		{
			MaxPriorityQueue queue;
			TestItem& item = testItems[0];

			queue.insert(makePair(item, 1));

			const TestItem& optimal = queue.extractOptimal();

			Assert::IsTrue(queue.isEmpty(), L"Queue is not empty after extracting the only item in it");
			Assert::IsTrue(item == optimal, L"Extracted optimal is different from the only inserted item");
		}

		TEST_METHOD(testExtractOptimalWithMoreThanOneItem)
		{
			MaxPriorityQueue queue;
			TestItem& smallest = testItems[0];
			TestItem& medium = testItems[1];
			TestItem& biggest = testItems[2];

			queue.insert(makePair(smallest, 0));
			queue.insert(makePair(medium, 1));
			queue.insert(makePair(biggest, 2));

			const TestItem& optimal = queue.extractOptimal();
			const TestItem& newOptimal = queue.getOptimal();

			Assert::IsTrue(biggest == optimal, L"Extracted optimal is not the correct one");
			Assert::IsTrue(medium == newOptimal, L"The new optimal item in the queue is not correct");
		}
	};

	PriorityQueueTest::TestItemArray PriorityQueueTest::testItems(TEST_ITEMS_COUNT, TEST_ITEMS_COUNT);
}