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
			list.insertAsTail(i);
	}

	void fillListAddingHead(List& list, int count)
	{
		for (int i = 0; i < count; ++i)
			list.insertAsHead(i);
	}

	bool doIteratorsPointToEqualLists(ListConstIterator& lhsHead, ListConstIterator& rhsHead)
	{
		while (lhsHead)
		{
			if (*lhsHead != *rhsHead)
				return false;

			++lhsHead;
			++rhsHead;
		}

		return !lhsHead && !rhsHead;
	}

	bool areEqual(const List& lhs, const List& rhs)
	{
		return (lhs.getCount() == rhs.getCount()) && doIteratorsPointToEqualLists(lhs.getHeadConstIterator(), rhs.getHeadConstIterator());
	}


	TEST_CLASS(ForwardListTest)
	{
	private:
		static const int NUMBER_OF_ITEMS_TO_INSERT = 100;

	public:
		TEST_METHOD(EmptyListTest)
		{
			List list;

			Assert::IsTrue(list.getCount() == 0, L"List is not empty when constructed");
			Assert::IsTrue(list.isEmpty(), L"IsEmpty returns false after construction");

			ListConstIterator iterator = list.getHeadConstIterator();
			Assert::IsFalse(iterator);

			iterator = list.getTailConstIterator();
			Assert::IsFalse(iterator);
		}
	
		TEST_METHOD(InsertTailTest)
		{
			List list;

			list.insertAsTail(1);

			Assert::AreEqual(list.getHead(), list.getTail());

			list.removeHead();

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				list.insertAsTail(i);
				Assert::IsTrue(list.getCount() == i + 1, L"Count is not set properly when inserting as tail");
				Assert::IsTrue(list.getTail() == i);
			}
		}

		TEST_METHOD(InsertHeadTest)
		{
			List list;

			list.insertAsHead(1);

			Assert::AreEqual(list.getHead(), list.getTail());

			list.removeHead();

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				list.insertAsHead(i);
				Assert::IsTrue(list.getCount() == i + 1, L"Count is not set properly when inserting as head");
				Assert::IsTrue(list.getHead() == i);
			}
		}

		TEST_METHOD(AppendingToListCountTest)
		{
			List destination;
			fillListAddingTail(destination, NUMBER_OF_ITEMS_TO_INSERT);

			const int destInitialCount = destination.getCount();

			List source;
			destination.appendList(source);

			Assert::IsTrue(destination.getCount() == destInitialCount, L"Appending an empty list changes count");

			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT / 2);
			destination.appendList(source);

			Assert::IsTrue(destination.getCount() == (destInitialCount + source.getCount()), L"Appending a non-empty list changes count incorrectly");
		}
		
		TEST_METHOD(AppendingToListReconstruction)
		{
			List destination;
			List source;

			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT);
			destination.appendList(source);

			Assert::IsTrue(areEqual(destination, source));

			ListConstIterator destinationTail = destination.getTailConstIterator();
			destination.appendList(source);

			++destinationTail;
			Assert::IsTrue(destinationTail, L"The node after tail is null after appending non-empty list");

			ListConstIterator sourceHead = source.getHeadConstIterator();

			Assert::IsTrue(doIteratorsPointToEqualLists(destinationTail, sourceHead));
		}

		TEST_METHOD(RemovingHeadTest)
		{
			List list;
			fillListAddingHead(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = NUMBER_OF_ITEMS_TO_INSERT; i > 0; --i)
			{
				Assert::IsTrue(list.getHead() == i - 1, L"Removing head does not manage head properly");
				Assert::IsTrue(list.getCount() == i, L"Removing head does not manage count properly");
				list.removeHead();
			}
		}
		
		TEST_METHOD(RemovingTailTest)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = NUMBER_OF_ITEMS_TO_INSERT; i > 0; --i)
			{
				Assert::IsTrue(list.getTail() == i - 1, L"Removing tail does not manage tail properly");
				Assert::IsTrue(list.getCount() == i, L"Removing tail does not manage count properly");
				list.removeTail();
			}
		}

		TEST_METHOD(InsertionAfterNullIterator)
		{
			List list;

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator iterator = list.getTailIterator();
				//forse null
				++iterator;

				//should insert it as tail
				list.insertAfter(iterator, i);

				Assert::IsTrue(list.getTail() == i);
			}
		}

		TEST_METHOD(InsertingAfterHeadIterator)
		{
			List list;
			list.insertAsHead(1);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator headIterator = list.getHeadIterator();
				list.insertAfter(headIterator, i);

				++headIterator;

				Assert::IsTrue(headIterator, L"Insertion after iterator does not update its successor");
				Assert::IsTrue(*headIterator == i, L"Insertion after iterator does not construct with the value passed");
			}
		}

		TEST_METHOD(InsertingBetweenNodesWithInsertAfterIterator)
		{
			List list;

			list.insertAsHead(1);
			list.insertAsTail(3);

			ListIterator headIterator = list.getHeadIterator();
			list.insertAfter(headIterator, 2);

			for (int i = 1; i < 4; ++i)
			{
				Assert::IsTrue(*headIterator == i, L"Inserting between two nodes with iterator is not working properly");
				++headIterator;
			}

			Assert::IsFalse(headIterator);
		}

		TEST_METHOD(InsertingAfterTailIterator)
		{
			List list;
			list.insertAsTail(1);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator tailIterator = list.getTailIterator();
				list.insertAfter(tailIterator, i);

				++tailIterator;

				Assert::IsTrue(tailIterator);
				Assert::IsTrue(*tailIterator == i, L"Insertion after tail iterator does not construct with the value passed");
			}
		}

		TEST_METHOD(InsertionBeforeNullIterator)
		{
			List list;

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				ListIterator iterator = list.getTailIterator();
				//forse null
				++iterator;

				//should insert it as head
				list.insertBefore(iterator, i);

				Assert::IsTrue(list.getHead() == i);
			}
		}

		TEST_METHOD(InsertingBeforeHeadIterator)
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

		TEST_METHOD(InsertingBetweenNodesWithInsertBeforeIterator)
		{
			List list;

			list.insertAsHead(1);
			list.insertAsTail(3);

			ListIterator iterator = list.getTailIterator();
			list.insertBefore(iterator, 2);

			iterator = list.getHeadIterator();
			for (int i = 1; i < 4; ++i)
			{
				Assert::IsTrue(*iterator == i);
				++iterator;
			}

			Assert::IsFalse(iterator);
		}

		TEST_METHOD(RemovingAtNullIterator)
		{
			List emptyList;

			ListIterator nullIterator = emptyList.getHeadIterator();
			emptyList.removeAt(nullIterator);

			Assert::IsTrue(emptyList.getCount() == 0);
			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(RemovingAtHeadIterator)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				Assert::IsTrue(list.getHead() == i, L"Removing at head iterator does not manage head properly");
				Assert::IsTrue(list.getCount() == NUMBER_OF_ITEMS_TO_INSERT - i, L"Removing at head iterator does not manage count properly");

				ListIterator headIterator = list.getHeadIterator();
				list.removeAt(headIterator);

				Assert::IsFalse(headIterator, L"Removing at head iterator does not null the iterator");
			}

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(RemovingAtTailIterator)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = NUMBER_OF_ITEMS_TO_INSERT - 1; i >= 0; --i)
			{
				Assert::IsTrue(list.getTail() == i, L"Removing at tail iterator does not manage tail properly");
				Assert::IsTrue(list.getCount() == i + 1, L"Removing at tail iterator does not manage count properly");

				ListIterator tailIterator = list.getTailIterator();
				list.removeAt(tailIterator);

				Assert::IsFalse(tailIterator, L"Removing at iterator does not null iterator");
			}

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(RemovingBetweenNodesWithIterator)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				//head stays 0 after removing its successor
				Assert::IsTrue(list.getHead() == 0, L"Removing between nodes with iterator does not manage predecessor properly");
				Assert::IsTrue(list.getCount() == NUMBER_OF_ITEMS_TO_INSERT - i, L"Removing between nodes with iterator does not manage count properly");

				ListIterator secondNodeIterator = list.getHeadIterator();
				++secondNodeIterator;

				//if at least two nodes in the list
				if (i < NUMBER_OF_ITEMS_TO_INSERT - 1)
				{
					Assert::IsTrue(*secondNodeIterator == i + 1, L"Removing between nodes with iterator does not manage successor properly");
				}

				list.removeAt(secondNodeIterator);
				Assert::IsFalse(secondNodeIterator, L"Removing at iterator does not null iterator");
			}

			Assert::IsTrue(list.getCount() == 1);
			Assert::IsTrue(list.getHead() == 0);
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