#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef SinglyLinkedList<int>::Iterator ListIterator;
typedef SinglyLinkedList<int>::ConstIterator ListConstIterator;
typedef SinglyLinkedList<int> List;

namespace SinglyLinkedListTest
{
	void fillListAddingTail(List& list, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			list.insertAsTail(i);
		}
	}

	void fillListAddingHead(List& list, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			list.insertAsHead(i);
		}
	}

	bool doIteratorsPointToEqualLists(ListConstIterator& lhsHead, ListConstIterator& rhsHead)
	{
		while (lhsHead)
		{
			if (*lhsHead != *rhsHead)
			{
				return false;
			}

			++lhsHead;
			++rhsHead;
		}

		return !lhsHead && !rhsHead;
	}

	bool areEqual(const List& lhs, const List& rhs)
	{
		return (lhs.getCount() == rhs.getCount()) && doIteratorsPointToEqualLists(lhs.getHeadConstIterator(), rhs.getHeadConstIterator());
	}


	TEST_CLASS(SinglyLinkedListTest)
	{
	private:
		static const int NUMBER_OF_ITEMS_TO_INSERT = 100;

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

			Assert::IsFalse(head);
			Assert::IsFalse(tail);
		}
	
		TEST_METHOD(testTailInsertionUpdatesCountAndTail)
		{
			List list;

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				list.insertAsTail(i);
				Assert::IsTrue(list.getCount() == i + 1, L"Count is not updated when inserting as tail");
				Assert::IsTrue(list.getTail() == i, L"Tail is not updated when inserting as tail");
			}
		}

		TEST_METHOD(testInsertionAsHeadUpdatesCountAndHead)
		{
			List list;

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				list.insertAsHead(i);
				Assert::IsTrue(list.getCount() == i + 1, L"Count is not updated when inserting as head");
				Assert::IsTrue(list.getHead() == i, L"Head is not updated when inserting as head");
			}
		}

		TEST_METHOD(testAppendListUpdatesCount)
		{
			List source;
			List destination;

			fillListAddingTail(destination, NUMBER_OF_ITEMS_TO_INSERT);
			const int destInitialCount = destination.getCount();

			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT / 2);
			destination.appendList(source);

			Assert::IsTrue(destination.getCount() == (destInitialCount + source.getCount()));
		}
		
		TEST_METHOD(testAppendList)
		{
			List source;
			List destination;

			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT);
			
			destination.appendList(source);
			ListConstIterator destinationIterator = destination.getTailConstIterator();

			destination.appendList(source);

			++destinationIterator;
			Assert::IsTrue(destinationIterator, L"There is no item after tail after appending non-empty list");

			ListConstIterator sourceIterator = source.getHeadConstIterator();

			Assert::IsTrue(doIteratorsPointToEqualLists(destinationIterator, sourceIterator));
		}

		TEST_METHOD(testRemoveHeadUpdatesCountAndHead)
		{
			List list;
			fillListAddingHead(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = NUMBER_OF_ITEMS_TO_INSERT; i > 0; --i)
			{
				Assert::IsTrue(list.getHead() == i - 1, L"Removing head does not update head");
				Assert::IsTrue(list.getCount() == i, L"Removing head does not update count");
				list.removeHead();
			}
		}
		
		TEST_METHOD(testRemoveTailUpdatesCoundAndTail)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = NUMBER_OF_ITEMS_TO_INSERT; i > 0; --i)
			{
				Assert::IsTrue(list.getTail() == i - 1, L"Removing tail does not update tail");
				Assert::IsTrue(list.getCount() == i, L"Removing tail does not update count");
				list.removeTail();
			}
		}

		TEST_METHOD(testInsertionAfterNullIteratorInsertsAsTail)
		{
			List list;

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator iterator = list.getTailIterator();

				//forse null
				++iterator;

				list.insertAfter(iterator, i);

				Assert::IsTrue(list.getTail() == i);
			}
		}

		TEST_METHOD(testInsertionAfterHeadIterator)
		{
			List list;
			list.insertAsHead(1);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator headIterator = list.getHeadIterator();

				list.insertAfter(headIterator, i);

				++headIterator;

				Assert::IsTrue(headIterator, L"There is no object after the object we inserted after");
				Assert::IsTrue(*headIterator == i, L"The inserted object is not the one passed as argument");
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
				Assert::IsTrue(*headIterator == i);
				++headIterator;
				++i;
			}
		}

		TEST_METHOD(testInsertionAfterTailIterator)
		{
			List list;
			list.insertAsTail(1);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator tailIterator = list.getTailIterator();

				list.insertAfter(tailIterator, i);

				++tailIterator;

				Assert::IsTrue(tailIterator, L"There is no object after the object we inserted after");
				Assert::IsTrue(*tailIterator == i, L"The inserted object is not the one passed as argument");
			}
		}

		TEST_METHOD(testInsertionBeforeNullIteratorInsertsAsHead)
		{
			List list;

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator iterator = list.getTailIterator();

				//forse null
				++iterator;

				list.insertBefore(iterator, i);

				Assert::IsTrue(list.getHead() == i);
			}
		}

		TEST_METHOD(testInsertionBeforeHeadIterator)
		{
			List list;

			list.insertAsHead(1);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator headIterator = list.getHeadIterator();

				list.insertBefore(headIterator, i);

				Assert::IsTrue(list.getHead() == i);
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
				Assert::IsTrue(*iterator == i);

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
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				Assert::IsTrue(list.getHead() == i, L"Removing at head iterator does not update head");
				Assert::IsTrue(list.getCount() == NUMBER_OF_ITEMS_TO_INSERT - i, L"Removing at head iterator does not update count");

				ListIterator headIterator = list.getHeadIterator();
				list.removeAt(headIterator);

				Assert::IsFalse(headIterator, L"Removing at head iterator does not invalidate the iterator");
			}
		}

		TEST_METHOD(testRemovalAtTailIterator)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = NUMBER_OF_ITEMS_TO_INSERT - 1; i >= 0; --i)
			{
				Assert::IsTrue(list.getTail() == i, L"Removing at tail iterator does not update tail");
				Assert::IsTrue(list.getCount() == i + 1, L"Removing at tail iterator does not update count");

				ListIterator tailIterator = list.getTailIterator();
				list.removeAt(tailIterator);

				Assert::IsFalse(tailIterator, L"Removing at iterator does not invalidate the iterator");
			}
		}

		TEST_METHOD(testRemovalBetweenElementsWithIterator)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			const int insertedItemsCount = NUMBER_OF_ITEMS_TO_INSERT;
			const int headElement = list.getHead();

			for (int i = 0; i < insertedItemsCount; ++i)
			{
				Assert::IsTrue(list.getHead() == headElement, L"Removing between first and third modifies first element");
				Assert::IsTrue(list.getCount() == insertedItemsCount - i, L"Removing between element with iterator does not update count");

				ListIterator secondElementIterator = list.getHeadIterator();
				++secondElementIterator;

				if (insertedItemsCount - i > 1)
				{
					Assert::IsTrue(*secondElementIterator == i + 1, L"Removing between nodes with iterator does not manage successor properly");
				}

				list.removeAt(secondElementIterator);
				Assert::IsFalse(secondElementIterator, L"Removing at iterator does not invalidate iterator");
			}

			Assert::IsTrue(list.getCount() == 1);
			Assert::IsTrue(list.getHead() == headElement);
		}

		TEST_METHOD(CopyCtorTest)
		{
			List source;

			List destinationOne(source);
			Assert::IsTrue(areEqual(source, destinationOne));

			fillListAddingHead(source, NUMBER_OF_ITEMS_TO_INSERT);

			List destinationTwo(source);
			Assert::IsTrue(areEqual(source, destinationTwo));
		}

		TEST_METHOD(MoveCtorFromEmpty)
		{
			List source;
			List destination(std::move(source));

			Assert::IsTrue(destination.getCount() == 0);
			Assert::IsTrue(destination.isEmpty());

			Assert::IsTrue(source.getCount() == 0);
			Assert::IsTrue(source.isEmpty());
		}

		TEST_METHOD(MoveCtorFromNonEmpty)
		{
			List source;
			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT);

			List initialSourceCopy(source);

			List destination(std::move(source));

			Assert::IsTrue(areEqual(destination, initialSourceCopy));

			Assert::IsTrue(source.getCount() == 0);
			Assert::IsTrue(source.isEmpty());
		}

		TEST_METHOD(CopyAssignmentEmptyToEmpty)
		{
			List lhs;
			List rhs;

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}

		TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		{
			List lhs;
			List rhs;

			fillListAddingTail(rhs, NUMBER_OF_ITEMS_TO_INSERT);

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}

		TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		{
			List lhs;
			List rhs;

			fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);
			fillListAddingTail(rhs, NUMBER_OF_ITEMS_TO_INSERT / 2);

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}

		TEST_METHOD(CopyAssignmentEmptyToNonEmpty)
		{
			List lhs;
			List rhs;

			fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);

			lhs = rhs;

			Assert::IsTrue(areEqual(lhs, rhs));
		}
		
		TEST_METHOD(MoveAssignmentEmptyToEmpty)
		{
			List lhs;
			List rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.getCount() == 0);
			Assert::IsTrue(lhs.isEmpty());
			Assert::IsTrue(rhs.getCount() == 0);
			Assert::IsTrue(rhs.isEmpty());
		}

		TEST_METHOD(MoveAssignmentNonEmptyToEmpty)
		{
			List lhs;
			List rhs;

			fillListAddingHead(rhs, NUMBER_OF_ITEMS_TO_INSERT);

			List initialRhsCopy(rhs);

			lhs = std::move(rhs);

			Assert::IsTrue(areEqual(lhs, initialRhsCopy));
			Assert::IsTrue(rhs.getCount() == 0);
			Assert::IsTrue(rhs.isEmpty());
		}
		
		TEST_METHOD(MoveAssignmentEmptyToNonEmpty)
		{
			List lhs;
			List rhs;

			fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.getCount() == 0);
			Assert::IsTrue(lhs.isEmpty());
			Assert::IsTrue(rhs.getCount() == 0);
			Assert::IsTrue(rhs.isEmpty());
		}


		TEST_METHOD(MoveAssignmentNonEmptyToNonEmpty)
		{
			List lhs;
			List rhs;

			fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);
			fillListAddingTail(rhs, NUMBER_OF_ITEMS_TO_INSERT / 2);

			List initialRhsCopy(rhs);

			lhs = std::move(rhs);

			Assert::IsTrue(areEqual(lhs, initialRhsCopy));
			Assert::IsTrue(rhs.getCount() == 0);
			Assert::IsTrue(rhs.isEmpty());
		}
	};
}