#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\..\..\Graph store\Priority Queue\PriorityQueue.h"
#include "..\..\..\Graph store\Dynamic Array\DArray.h"

namespace PriorityQueueTest
{	
	class TestItem
	{
	public:
		TestItem(unsigned ID = 0) : id{ ID } {}

		void setHandle(const PriorityQueueHandle& handle) { this->handle = handle; }
		const PriorityQueueHandle& getHandle() const { return handle; }

		void setID(unsigned ID) { id = ID; }
		unsigned getID() const { return id; }

	private:
		unsigned id{};
		PriorityQueueHandle handle;
	};

	bool operator==(const TestItem& lhs, const TestItem& rhs) { return lhs.getID() == rhs.getID(); }
	bool operator!=(const TestItem& lhs, const TestItem& rhs) { return !(lhs == rhs); }

	struct KeyAccessor
	{
		static void setKeyOf(TestItem& item, unsigned ID) { item.setID(ID); }
		static unsigned getKeyOf(const TestItem& item) { return item.getID(); }
	};

	struct HandleSetter
	{
		void operator()(TestItem& item, const PriorityQueueHandle& handle) const
		{
			item.setHandle(handle);
		}
	};

	TEST_CLASS(PriorityQueueTest)
	{
	private:
		using MaxPriorityQueue = PriorityQueue<TestItem, unsigned, KeyAccessor, std::less<unsigned>, HandleSetter>;
		using TestItemArray = DArray<TestItem>;

		static TestItemArray testItems;
		static const size_t TEST_ITEMS_COUNT = 8;

	private:
		static void insertTestItemsInRangeByID(MaxPriorityQueue& queue, int smallest, int biggest)
		{
			assert(isValidRange(smallest, biggest));

			for (int i = smallest; i <= biggest; ++i)
			{
				queue.insert(testItems[i]);
			}
		}

		static bool containsItemsInRange(MaxPriorityQueue& queue, int smallest, int biggest)
		{
			assert(isValidRange(smallest, biggest));

			for (int i = biggest; i >= smallest; --i)
			{
				auto& extracted = queue.extractOptimal();

				if (extracted != testItems[i])
				{
					return false;
				}
			}

			return queue.isEmpty();
		}

		static bool isValidRange(int smallest, int biggest)
		{
			return smallest <= biggest && biggest < TEST_ITEMS_COUNT;
		}

	public:	
		TEST_METHOD_INITIALIZE(initializeItemsWithIncreasingIDs)
		{
			auto i = 0U;
			for (auto&& item : testItems)
			{
				item.setID(i++);
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
			auto middle = TEST_ITEMS_COUNT / 2;
			auto end = TEST_ITEMS_COUNT - 1;

			insertTestItemsInRangeByID(queue, middle, end);
			insertTestItemsInRangeByID(queue, 0, middle - 1);
			
			auto& optimal = queue.getOptimal();

			Assert::IsTrue(optimal == testItems[end]);
		}

		TEST_METHOD(testImproveKeyToOptimalRearangesInsertedItems)
		{
			MaxPriorityQueue queue;
			auto& expectedOptimal = testItems[0];

			insertTestItemsInRangeByID(queue, 0, 2);

			queue.improveKey(expectedOptimal.getHandle(), testItems[2].getID() + 1);
			auto& optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(testImproveKeyToNonOptimalDoesNotChangeTheOptimal)
		{
			MaxPriorityQueue queue;
			auto& expectedOptimal = testItems[3];
			
			insertTestItemsInRangeByID(queue, 0, 3);

			queue.improveKey(testItems[0].getHandle(), expectedOptimal.getID() - 1);
			auto& optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(testImproveKeyToNewOptimalDoesNotChangeTheOptimal)
		{
			MaxPriorityQueue queue;
			auto& expectedOptimal = testItems[2];

			insertTestItemsInRangeByID(queue, 0, 2);

			queue.improveKey(expectedOptimal.getHandle(), expectedOptimal.getID() + 1);
			auto& optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(testExtractOptimalWithJustOneItem)
		{
			MaxPriorityQueue queue;
			auto& expected = testItems[0];

			insertTestItemsInRangeByID(queue, 0, 0);
	
			auto& optimal = queue.extractOptimal();

			Assert::IsTrue(queue.isEmpty(), L"Queue is not empty after extracting the only item in it");
			Assert::IsTrue(expected == optimal, L"Extracted optimal is different from the only inserted item");
		}

		TEST_METHOD(testSimpleExtractOptimalWithSeveralItems)
		{
			MaxPriorityQueue queue;
			auto end = TEST_ITEMS_COUNT - 1;

			insertTestItemsInRangeByID(queue, 0, end);

			auto& optimal = queue.extractOptimal();
			auto& newOptimal = queue.getOptimal();
			
			Assert::IsTrue(testItems[end] == optimal, L"Extracted optimal is not the correct one");
			Assert::IsTrue(testItems[end - 1] == newOptimal, L"The new optimal item in the queue is not correct");
		}

		TEST_METHOD(testCtorFromRange)
		{
			using namespace std;
			MaxPriorityQueue queue{ begin(testItems), end(testItems) };

			Assert::IsTrue(containsItemsInRange(queue, 0, TEST_ITEMS_COUNT - 1));
		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			MaxPriorityQueue source;
			MaxPriorityQueue destination(source);

			Assert::IsTrue(destination.isEmpty());
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			MaxPriorityQueue source;
			insertTestItemsInRangeByID(source, 0, TEST_ITEMS_COUNT - 1);

			MaxPriorityQueue destination(source);

			Assert::IsTrue(containsItemsInRange(destination, 0, TEST_ITEMS_COUNT - 1));
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			MaxPriorityQueue source;
			MaxPriorityQueue destination(std::move(source));

			Assert::IsTrue(source.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(destination.isEmpty(), L"Moved-in object is not empty after moving an empty object in it");
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			MaxPriorityQueue source;
			insertTestItemsInRangeByID(source, 1, TEST_ITEMS_COUNT - 1);

			MaxPriorityQueue destination(std::move(source));

			Assert::IsTrue(source.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(containsItemsInRange(destination, 1, TEST_ITEMS_COUNT - 1), L"Moved-in object has wrong contents");
		}

		TEST_METHOD(testCopyAssignmentFromEmptyRhsToEmptyLhs)
		{
			MaxPriorityQueue lhs;
			MaxPriorityQueue rhs;

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

		TEST_METHOD(testCopyAssignmentFromNonEmptyRhsToEmptyLhs)
		{
			MaxPriorityQueue lhs;
			MaxPriorityQueue rhs;
			insertTestItemsInRangeByID(rhs, 1, 4);

			lhs = rhs;

			Assert::IsTrue(containsItemsInRange(lhs, 1, 4));
		}

		TEST_METHOD(testCopyAssignmentFromNonEmptyRhsToNonEmptyLhs)
		{
			MaxPriorityQueue rhs;
			MaxPriorityQueue lhs;
			insertTestItemsInRangeByID(lhs, 0, 2);
			insertTestItemsInRangeByID(rhs, 1, 4);

			lhs = rhs;

			Assert::IsTrue(containsItemsInRange(lhs, 1, 4));
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

		TEST_METHOD(testMoveAssignmentFromNonEmptyRhsToEmptyLhs)
		{
			MaxPriorityQueue lhs;
			MaxPriorityQueue rhs;
			insertTestItemsInRangeByID(rhs, 1, 3);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(containsItemsInRange(lhs, 1, 3), L"Moved-in object has wrong contents");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyRhsToNonEmptyLhs)
		{
			MaxPriorityQueue lhs;
			MaxPriorityQueue rhs;
			insertTestItemsInRangeByID(lhs, 0, 3);
			insertTestItemsInRangeByID(rhs, 1, 5);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from object is not empty");
			Assert::IsTrue(containsItemsInRange(lhs, 1, 5), L"Moved-in object has wrong contents");
		}
	};

	PriorityQueueTest::TestItemArray PriorityQueueTest::testItems(TEST_ITEMS_COUNT, TEST_ITEMS_COUNT);
}