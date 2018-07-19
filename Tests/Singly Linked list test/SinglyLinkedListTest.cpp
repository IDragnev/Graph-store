#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef SinglyLinkedList<int>::Iterator ListIterator;
typedef SinglyLinkedList<int>::ConstIterator ListConstIterator;
typedef SinglyLinkedList<int> List;

namespace SinglyLinkedListTest
{
	void fillListWithIntegersFromZeroTo(List& list, const int integer)
	{
		for (int i = 0; i <= integer; ++i)
		{
			list.insertAsTail(i);
		}
	}

	void fillListWithIntegersFromZeroToInReverse(List& list, const int integer)
	{
		for (int i = 0; i <= integer; ++i)
		{
			list.insertAsHead(i);
		}
	}

	bool iterateSameContents(ListConstIterator& lhs, ListConstIterator& rhs)
	{
		while (lhs)
		{
			if (*lhs != *rhs)
			{
				return false;
			}

			++lhs;
			++rhs;
		}

		return !lhs && !rhs;
	}

	bool areEqual(const List& lhs, const List& rhs)
	{
		return (lhs.getCount() == rhs.getCount()) && iterateSameContents(lhs.getHeadConstIterator(), rhs.getHeadConstIterator());
	}

	bool containsAllIntegersFromZeroTo(const List& list, const int integer)
	{
		ListConstIterator constIterator = list.getHeadConstIterator();
		int currentInteger = 0;

		while (constIterator)
		{
			if (*constIterator != currentInteger)
			{
				return false;
			}

			++constIterator;
			++currentInteger;
		}

		return currentInteger - 1 == integer;
	}


	TEST_CLASS(SinglyLinkedListTest)
	{
	private:
		static const int BIG_INTEGER = 100;

	public:
		TEST_METHOD(testDefaultConstructedListIsEmpty)
		{
			List list;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testEmptyListReturnsNullIterators)
		{
			List list;

			ListConstIterator head = list.getHeadConstIterator();
			ListConstIterator tail = list.getTailConstIterator();

			Assert::IsFalse(head, L"Head iterator of empty list is not null");
			Assert::IsFalse(tail, L"Tail iterator of empty list is not null");
		}
	
		TEST_METHOD(testTailInsertionUpdatesCountAndTail)
		{
			List list;

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				list.insertAsTail(i);

				Assert::AreEqual(list.getCount(), i + 1U, L"Count is not updated when inserting as tail");
				Assert::AreEqual(list.getTail(), i, L"Tail is not updated when inserting as tail");
			}
		}

		TEST_METHOD(testInsertionAsHeadUpdatesCountAndHead)
		{
			List list;

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				list.insertAsHead(i);

				Assert::AreEqual(list.getCount(), i + 1U, L"Count is not updated when inserting as head");
				Assert::AreEqual(list.getHead(), i, L"Head is not updated when inserting as head");
			}
		}
		
		TEST_METHOD(testAppendList)
		{
			List source;
			List destination;
			fillListWithIntegersFromZeroTo(source, BIG_INTEGER);
			fillListWithIntegersFromZeroTo(destination, BIG_INTEGER / 2);

			ListConstIterator destinationIterator = destination.getTailConstIterator();
			const int destinationInitialCount = destination.getCount();
			ListConstIterator sourceIterator = source.getHeadConstIterator();

			destination.appendList(source);
			++destinationIterator;

			Assert::IsTrue(destinationIterator, L"There is no item after tail after appending non-empty list");
			Assert::AreEqual(destination.getCount(), destinationInitialCount + source.getCount(), L"Appending list does not update count");
			Assert::IsTrue(iterateSameContents(destinationIterator, sourceIterator), L"Appended list is not the same in destination");
		}

		TEST_METHOD(testRemoveHeadUpdatesCountAndHead)
		{
			List list;
			fillListWithIntegersFromZeroToInReverse(list, BIG_INTEGER - 1);

			for (int i = BIG_INTEGER; i > 0; --i)
			{
				Assert::AreEqual(list.getHead(), i - 1, L"Removing head does not update head");
				Assert::IsTrue(list.getCount() == i, L"Removing head does not update count");

				list.removeHead();
			}
		}
		
		TEST_METHOD(testRemoveTailUpdatesCoundAndTail)
		{
			List list;
			fillListWithIntegersFromZeroTo(list, BIG_INTEGER - 1);

			for (int i = BIG_INTEGER; i > 0; --i)
			{
				Assert::AreEqual(list.getTail(), i - 1, L"Removing tail does not update tail");
				Assert::IsTrue(list.getCount() == i, L"Removing tail does not update count");

				list.removeTail();
			}
		}

		TEST_METHOD(testInsertionAfterNullIteratorInsertsAsTail)
		{
			List list;

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				ListIterator iterator = list.getTailIterator();
				//forse null
				++iterator;

				list.insertAfter(iterator, i);

				Assert::AreEqual(list.getTail(), i);
			}
		}

		TEST_METHOD(testInsertionAfterHeadIterator)
		{
			List list;
			list.insertAsHead(1);

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				ListIterator headIterator = list.getHeadIterator();

				list.insertAfter(headIterator, i);

				++headIterator;

				Assert::IsTrue(headIterator, L"There is no object after the object we inserted after");
				Assert::AreEqual(*headIterator, i, L"The inserted object is not the one passed as argument");
			}
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertAfterIterator)
		{
			List list;
			list.insertAsHead(1);
			list.insertAsTail(3);

			ListIterator headIterator = list.getHeadIterator();
			list.insertAfter(headIterator, 2);

			int i = 1;
			while(headIterator)
			{
				Assert::AreEqual(*headIterator, i);

				++headIterator;
				++i;
			}
		}

		TEST_METHOD(testInsertionAfterTailIterator)
		{
			List list;
			list.insertAsTail(1);

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				ListIterator tailIterator = list.getTailIterator();

				list.insertAfter(tailIterator, i);

				++tailIterator;

				Assert::IsTrue(tailIterator, L"There is no object after the object we inserted after");
				Assert::AreEqual(*tailIterator, i, L"The inserted object is not the one passed as argument");
			}
		}

		TEST_METHOD(testInsertionBeforeNullIteratorInsertsAsHead)
		{
			List list;

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				ListIterator iterator = list.getTailIterator();
				//forse null
				++iterator;

				list.insertBefore(iterator, i);

				Assert::AreEqual(list.getHead(), i);
			}
		}

		TEST_METHOD(testInsertionBeforeHeadIterator)
		{
			List list;
			list.insertAsHead(1);

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				ListIterator headIterator = list.getHeadIterator();

				list.insertBefore(headIterator, i);

				Assert::AreEqual(list.getHead(), i);
			}
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertBeforeIterator)
		{
			List list;
			list.insertAsHead(1);
			list.insertAsTail(3);

			ListIterator iterator = list.getTailIterator();

			list.insertBefore(iterator, 2);

			iterator = list.getHeadIterator();
			int i = 1;
			while(iterator)
			{
				Assert::AreEqual(*iterator, i);

				++iterator;
				++i;
			}
		}

		TEST_METHOD(testRemovalAtNullIteratorDoesNothing)
		{
			List emptyList;
			ListIterator nullIterator = emptyList.getHeadIterator();

			emptyList.removeAt(nullIterator);

			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(testRemovalAtHeadIterator)
		{
			List list;
			fillListWithIntegersFromZeroTo(list, BIG_INTEGER - 1);

			for (int i = 0; i < BIG_INTEGER; ++i)
			{
				ListIterator headIterator = list.getHeadIterator();
				list.removeAt(headIterator);

				Assert::IsFalse(headIterator, L"Removing at head iterator does not invalidate the iterator");

				if (!list.isEmpty())
				{
					Assert::AreEqual(list.getHead(), i + 1, L"Removing at head iterator does not update head");
				}

				Assert::IsTrue(list.getCount() == BIG_INTEGER - (i + 1), L"Removing at head iterator does not update count");
			}
		}

		TEST_METHOD(testRemovalAtTailIterator)
		{
			List list;
			fillListWithIntegersFromZeroTo(list, BIG_INTEGER - 1);

			for (int i = BIG_INTEGER - 1; i >= 0; --i)
			{
				ListIterator tailIterator = list.getTailIterator();
				list.removeAt(tailIterator);

				Assert::IsFalse(tailIterator, L"Removing at iterator does not invalidate the iterator");

				if (!list.isEmpty())
				{
					Assert::AreEqual(list.getTail(), i - 1, L"Removing at tail iterator does not update tail");
				}

				Assert::IsTrue(list.getCount() == i, L"Removing at tail iterator does not update count");
			}
		}

		TEST_METHOD(testRemovalBetweenElementsWithIterator)
		{
			List list;
			fillListWithIntegersFromZeroTo(list, 2);

			ListIterator iterator = list.getHeadIterator();
			++iterator;

			list.removeAt(iterator);

			Assert::IsFalse(iterator, L"Iterator is not invalidated after removing at it");
			Assert::AreEqual(list.getCount(), 2U, L"Removing at iterator does not update count");
			Assert::AreEqual(list.getTail(), 2, L"Removing between elements does not update successor");
		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			List source;
			List destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			List source;
			fillListWithIntegersFromZeroTo(source, BIG_INTEGER);

			List destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			List source;
			List destination(std::move(source));

			Assert::IsTrue(destination.isEmpty(), L"Destination is not empty after moving an empty source in it");
			Assert::IsTrue(source.isEmpty(), L"Source is not empty after moving it");
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			List source;
			fillListWithIntegersFromZeroTo(source, BIG_INTEGER);

			List destination(std::move(source));

			Assert::IsTrue(containsAllIntegersFromZeroTo(destination, BIG_INTEGER), L"Destination has different contents from the moved-in source");
			Assert::IsTrue(source.isEmpty(), L"Source is not empty after moving it");
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			List lhs;
			List rhs;

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			List lhs;
			List rhs;
			fillListWithIntegersFromZeroTo(rhs, BIG_INTEGER);

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			List lhs;
			List rhs;
			fillListWithIntegersFromZeroToInReverse(lhs, BIG_INTEGER);
			fillListWithIntegersFromZeroTo(rhs, BIG_INTEGER / 2);

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			List lhs;
			List rhs;
			fillListWithIntegersFromZeroTo(lhs, BIG_INTEGER);

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}
		
		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			List lhs;
			List rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Lhs is not empty after move-assigning it an empty rhs");
			Assert::IsTrue(rhs.isEmpty(), L"Rhs is not empty after move-assigning it");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			List lhs;
			List rhs;
			fillListWithIntegersFromZeroTo(rhs, BIG_INTEGER);

			lhs = std::move(rhs);

			Assert::IsTrue(containsAllIntegersFromZeroTo(lhs, BIG_INTEGER), L"Lhs has different contents from the move-assignmed-from Rhs");
			Assert::IsTrue(rhs.isEmpty(), L"Rhs is not empty after move-assigning it");
		}
		
		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			List lhs;
			List rhs;
			fillListWithIntegersFromZeroTo(lhs, BIG_INTEGER);

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Lhs is not empty after move-assigning it an empty rhs");
			Assert::IsTrue(rhs.isEmpty(), L"Rhs is not empty after move-assigning it");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			List lhs;
			List rhs;
			fillListWithIntegersFromZeroToInReverse(lhs, BIG_INTEGER - 5);
			fillListWithIntegersFromZeroTo(rhs, BIG_INTEGER);

			lhs = std::move(rhs);

			Assert::IsTrue(containsAllIntegersFromZeroTo(lhs, BIG_INTEGER), L"Lhs has different contents from the move-assignmed-from Rhs");
			Assert::IsTrue(rhs.isEmpty(), L"Rhs is not empty after move-assigning it");
		}
	};
}