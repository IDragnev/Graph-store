#include "CppUnitTest.h"
#include "..\..\..\Graph store\Containers\Priority Queue\PriorityQueue.h"
#include "..\..\..\Graph store\Containers\Dynamic Array\DArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Containers::DArray;
using IDragnev::Containers::PriorityQueue;
using IDragnev::Containers::PriorityQueueHandle;

namespace PriorityQueueTest
{	
	TEST_CLASS(PriorityQueueTest)
	{
	private:
		using Handle = PriorityQueueHandle;

		struct TestItem
		{
			unsigned id = 0;
			Handle handle{};

			friend bool operator==(const TestItem& lhs, const TestItem& rhs)
			{
				return lhs.id == rhs.id;
			}

			friend bool operator!=(const TestItem& lhs, const TestItem& rhs)
			{
				return !(lhs == rhs);
			}
		};

		struct KeyAccessor
		{
			void operator()(TestItem& item, const unsigned& ID) const noexcept { item.id = ID; }
			const unsigned& operator()(const TestItem& item) const noexcept { return item.id; }
		};

		struct HandleSetter
		{
			void operator()(TestItem& item, const Handle& h) const noexcept
			{
				item.handle = h;
			}
		};

		using IntMaxPriorityQueue = PriorityQueue<int>;
		using IntArray = DArray<int>;

		using TestItemRef = std::reference_wrapper<TestItem>;
		using MaxPriorityQueue = PriorityQueue<TestItemRef, unsigned, KeyAccessor, IDragnev::Functional::LessThan, HandleSetter>;
		using TestItemArray = DArray<TestItem>;

		static TestItemArray testItems;
		static const size_t TEST_ITEMS_COUNT = 8;

	public:	
		TEST_METHOD_INITIALIZE(initializeItemsWithIncreasingIDs)
		{
			auto i = 0u;
			for (auto& item : testItems)
			{
				item.id = i++;
			}
		}

		TEST_METHOD(DefaultConstructedQueueIsEmpty)
		{
			MaxPriorityQueue queue;
			
			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(InsertArrangesTheItemsCorrectly)
		{
			MaxPriorityQueue queue;
			auto middle = TEST_ITEMS_COUNT / 2;
			auto end = TEST_ITEMS_COUNT - 1;

			insertTestItemsInRangeByID(queue, middle, end);
			insertTestItemsInRangeByID(queue, 0, middle - 1);
			
			auto optimal = queue.getOptimal();

			Assert::IsTrue(optimal == testItems[end]);
		}

		TEST_METHOD(ImproveKeyToOptimalRearangesInsertedItems)
		{
			auto queue = MaxPriorityQueue{};
			insertTestItemsInRangeByID(queue, 0, 2);
			auto& expectedOptimal = testItems[0];

			queue.improveKey(expectedOptimal.handle, testItems[2].id + 1);
			auto optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(ImproveKeyToNonOptimalDoesNotChangeTheOptimal)
		{
			auto queue = MaxPriorityQueue{};
			insertTestItemsInRangeByID(queue, 0, 3);
			auto& expectedOptimal = testItems[3];

			queue.improveKey(testItems[0].handle, expectedOptimal.id - 1);
			auto optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(ImproveKeyToNewOptimalDoesNotChangeTheOptimal)
		{
			auto queue = MaxPriorityQueue{};
			insertTestItemsInRangeByID(queue, 0, 2);
			auto& expectedOptimal = testItems[2];

			queue.improveKey(expectedOptimal.handle, expectedOptimal.id + 1);
			auto optimal = queue.getOptimal();

			Assert::IsTrue(expectedOptimal == optimal);
		}

		TEST_METHOD(ExtractOptimalWithJustOneItem)
		{
			auto queue = MaxPriorityQueue{};
			insertTestItemsInRangeByID(queue, 0, 0);
			auto& expected = testItems[0];
	
			auto optimal = queue.extractOptimal();

			Assert::IsTrue(queue.isEmpty(), L"Queue is not empty");
			Assert::IsTrue(expected == optimal, L"Extracted optimal is different from the only inserted item");
		}

		TEST_METHOD(SimpleExtractOptimalWithSeveralItems)
		{
			auto queue = MaxPriorityQueue{};
			auto end = TEST_ITEMS_COUNT - 1;
			insertTestItemsInRangeByID(queue, 0, end);

			auto optimal = queue.extractOptimal();
			auto newOptimal = queue.getOptimal();
			
			Assert::IsTrue(testItems[end] == optimal, L"Extracted optimal is not the correct one");
			Assert::IsTrue(testItems[end - 1] == newOptimal, L"The new optimal item in the queue is not correct");
		}

		TEST_METHOD(CtorFromRange)
		{
			using std::begin;
			using std::end;
			
			MaxPriorityQueue queue{ begin(testItems), end(testItems) };

			Assert::IsTrue(containsItemsInRange(queue, 0, TEST_ITEMS_COUNT - 1));
		}

		TEST_METHOD(InitListCtor)
		{
			MaxPriorityQueue queue{ testItems[0], testItems[1], testItems[2] };

			Assert::IsTrue(containsItemsInRange(queue, 0, 2));
		}

		TEST_METHOD(MoveCtorFromEmptySource)
		{
			MaxPriorityQueue source;
			auto destination = MaxPriorityQueue{ std::move(source) };

			Assert::IsTrue(source.isEmpty(), L"Moved-from queue is not empty");
			Assert::IsTrue(destination.isEmpty(), L"Moved-in queue is not empty");
		}

		TEST_METHOD(MoveCtorFromNonEmptySource)
		{
			MaxPriorityQueue source;
			insertTestItemsInRangeByID(source, 1, TEST_ITEMS_COUNT - 1);

			auto destination = MaxPriorityQueue{ std::move(source) };

			Assert::IsTrue(source.isEmpty(), L"Moved-from queue is not empty");
			Assert::IsTrue(containsItemsInRange(destination, 1, TEST_ITEMS_COUNT - 1), L"Moved-in queue has invalid contents");
		}

		TEST_METHOD(MoveAssignmentFromEmptyRhsToEmptyLhs)
		{
			auto lhs = MaxPriorityQueue{};
			auto rhs = MaxPriorityQueue{};

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from queue is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-in queue is not empty");
		}

		TEST_METHOD(MoveAssignmentFromEmptyRhsToNonEmptyLhs)
		{
			auto rhs = MaxPriorityQueue{};
			auto lhs = MaxPriorityQueue{};
			insertTestItemsInRangeByID(lhs, 0, 3);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from queue is not empty");
			Assert::IsTrue(lhs.isEmpty(), L"Moved-in queue is not empty");
		}

		TEST_METHOD(MoveAssignmentFromNonEmptyRhsToEmptyLhs)
		{
			auto rhs = MaxPriorityQueue{};
			auto lhs = MaxPriorityQueue{};
			insertTestItemsInRangeByID(rhs, 1, 3);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from queue is not empty");
			Assert::IsTrue(containsItemsInRange(lhs, 1, 3), L"Moved-in queue has invalid contents");
		}

		TEST_METHOD(MoveAssignmentFromNonEmptyRhsToNonEmptyLhs)
		{
			auto rhs = MaxPriorityQueue{};
			auto lhs = MaxPriorityQueue{};
			insertTestItemsInRangeByID(lhs, 0, 3);
			insertTestItemsInRangeByID(rhs, 1, 5);

			lhs = std::move(rhs);

			Assert::IsTrue(rhs.isEmpty(), L"Moved-from queue is not empty");
			Assert::IsTrue(containsItemsInRange(lhs, 1, 5), L"Moved-in queue has wrong contents");
		}

		//Queues storing pointers or references do not support copy semantics
		TEST_METHOD(CopyAssignmentFromEmptyRhsToEmptyLhs)
		{
			auto lhs = IntMaxPriorityQueue{};
			auto rhs = IntMaxPriorityQueue{};

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(CopyAssignmentFromEmptyRhsToNonEmptyLhs)
		{
			auto lhs = IntMaxPriorityQueue{ 1, 2, 3, 4, 5 };
			auto rhs = IntMaxPriorityQueue{};

			lhs = rhs;

			Assert::IsTrue(lhs.isEmpty());
		}

		TEST_METHOD(CopyAssignmentFromNonEmptyRhsToEmptyLhs)
		{
			auto lhs = IntMaxPriorityQueue{};
			auto rhs = IntMaxPriorityQueue{ 2, 1, 4, 3, 5, 6 };

			lhs = rhs;

			Assert::IsTrue(extractAll(lhs) == IntArray{ 6, 5, 4, 3, 2, 1 });
		}

		TEST_METHOD(CopyAssignmentFromNonEmptyRhsToNonEmptyLhs)
		{
			auto lhs = IntMaxPriorityQueue{ 4, 5, 6, 7, 8, 9 };
			auto rhs = IntMaxPriorityQueue{ 2, 1, 4, 3, 5, 6 };
	
			lhs = rhs;

			Assert::IsTrue(extractAll(lhs) == IntArray{ 6, 5, 4, 3, 2, 1 });
		}

		TEST_METHOD(CopyCtorFromEmptySource)
		{
			auto source = IntMaxPriorityQueue{};
			IntMaxPriorityQueue destination{ source };

			Assert::IsTrue(destination.isEmpty());
		}
	
		TEST_METHOD(CopyCtorFromNonEmptySource)
		{
			auto source = IntMaxPriorityQueue{ 2, 4, 1, 3, 5, 6 };
			IntMaxPriorityQueue destination{ source };

			Assert::IsTrue(extractAll(destination) == IntArray{ 6, 5, 4, 3, 2, 1 });
		}

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

				for (auto i = biggest; i >= smallest; --i)
				{
					auto extracted = queue.extractOptimal();

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

			static IntArray extractAll(IntMaxPriorityQueue& queue)
			{
				auto result = IntArray{};

				while (!queue.isEmpty())
				{
					result.insertBack(queue.extractOptimal());
				}

				return result;
			}
	};

	PriorityQueueTest::TestItemArray PriorityQueueTest::testItems(TEST_ITEMS_COUNT, TEST_ITEMS_COUNT);
}