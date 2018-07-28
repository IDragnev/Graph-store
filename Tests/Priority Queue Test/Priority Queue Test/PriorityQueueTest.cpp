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
		typedef Pair<TestItem, unsigned> TestPair;
		typedef DArray<TestPair> TestPairArray;
		typedef TestPairArray::Iterator TestPairIterator;

		static TestItemArray testItems;
		static const size_t TEST_ITEMS_COUNT = 8;

	private:
		static void insertTestItemsInRangeByID(MaxPriorityQueue& queue, unsigned smallest, unsigned biggest)
		{
			assert(isValidRange(smallest, biggest));

			for (unsigned i = 0; i <= biggest; ++i)
			{
				queue.insert(makePair(testItems[i]));
			}
		}

		static TestPair makePair(TestItem& item)
		{
			return TestPair(&item, item.getID());
		}

		static bool isValidRange(unsigned smallest, unsigned biggest)
		{
			return smallest <= biggest && biggest < TEST_ITEMS_COUNT;
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

		TEST_METHOD(testCtorFromNullIteratorAndZeroCreatesEmptyQueue)
		{
			TestPairArray emptyArray;
			TestPairIterator nullIterator = emptyArray.getHeadIterator();		
			MaxPriorityQueue queue(nullIterator, 0);

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testInsertArrangesTheItemsCorrectly)
		{
			MaxPriorityQueue queue;
			unsigned middle = TEST_ITEMS_COUNT / 2;
			unsigned end = TEST_ITEMS_COUNT - 1;

			insertTestItemsInRangeByID(queue, middle, end);
			insertTestItemsInRangeByID(queue, 0, middle - 1);
			
			const TestItem& optimal = queue.getOptimal();

			Assert::IsTrue(optimal == testItems[end]);
		}

		TEST_METHOD(testImproveKeyToOptimalRearangesInsertedItems)
		{
			MaxPriorityQueue queue;
			const TestItem& expectedOptimal = testItems[0];

			insertTestItemsInRangeByID(queue, 0, 2);

			queue.improveKey(expectedOptimal.getHandle(), testItems[2].getID() + 1);
			const TestItem& optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(testImproveKeyToNonOptimalDoesNotChangeTheOptimal)
		{
			MaxPriorityQueue queue;
			const TestItem& expectedOptimal = testItems[2];
			
			insertTestItemsInRangeByID(queue, 0, 2);

			queue.improveKey(testItems[0].getHandle(), expectedOptimal.getID() - 1);
			const TestItem& optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(testImproveKeyToNewOptimalDoesNotChangeTheOptimal)
		{
			MaxPriorityQueue queue;
			const TestItem& expectedOptimal = testItems[2];

			insertTestItemsInRangeByID(queue, 0, 2);

			queue.improveKey(expectedOptimal.getHandle(), expectedOptimal.getID() + 1);
			const TestItem& optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(testExtractOptimalWithJustOneItem)
		{
			MaxPriorityQueue queue;
			TestItem& expected = testItems[0];

			insertTestItemsInRangeByID(queue, 0, 0);

			const TestItem& optimal = queue.extractOptimal();

			Assert::IsTrue(queue.isEmpty(), L"Queue is not empty after extracting the only item in it");
			Assert::IsTrue(expected == optimal, L"Extracted optimal is different from the only inserted item");
		}

		TEST_METHOD(testSimpleExtractOptimalWithSeveralItems)
		{
			MaxPriorityQueue queue;
			const size_t end = TEST_ITEMS_COUNT - 1;

			insertTestItemsInRangeByID(queue, 0, end);

			const TestItem& optimal = queue.extractOptimal();
			const TestItem& newOptimal = queue.getOptimal();

			Assert::IsTrue(testItems[end] == optimal, L"Extracted optimal is not the correct one");
			Assert::IsTrue(testItems[end - 1] == newOptimal, L"The new optimal item in the queue is not correct");
		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			MaxPriorityQueue source;
			MaxPriorityQueue destination(source);

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			MaxPriorityQueue source;
			MaxPriorityQueue destination(std::move(source));

			Assert::IsTrue(source.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(destination.isEmpty(), L"Moved-in object is not empty after moving an empty object in it");
		}

		TEST_METHOD(testCopyAssignmentFromEmptyRhsToEmptyLhs)
		{
			MaxPriorityQueue rhs;
			MaxPriorityQueue lhs;

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentFromEmptyRhsToNonEmptyLhs)
		{
			MaxPriorityQueue rhs;
			MaxPriorityQueue lhs;
			insertTestItemsInRangeByID(lhs, 0, 2);

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(testMoveAssignmentFromEmptyRhsToEmptyLhs)
		{
			MaxPriorityQueue lhs;
			MaxPriorityQueue rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-in object is not empty after moving an empty object in it");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyRhsToNonEmptyLhs)
		{
			MaxPriorityQueue rhs;
			MaxPriorityQueue lhs;
			insertTestItemsInRangeByID(lhs, 0, 3);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-in object is not empty after moving an empty object in it");
		}
	};

	PriorityQueueTest::TestItemArray PriorityQueueTest::testItems(TEST_ITEMS_COUNT, TEST_ITEMS_COUNT);
}