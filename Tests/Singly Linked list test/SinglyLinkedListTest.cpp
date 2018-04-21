#include "CppUnitTest.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"
#include <algorithm>

typedef SinglyLinkedListIterator<int> ListIterator;
typedef SinglyLinkedList<int> List;
typedef std::unique_ptr<ListIterator> IteratorPtr;

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

	bool doIteratorsPointToEqualLists(ListIterator& lhsHead, ListIterator& rhsHead)
	{
		while (!lhsHead.isFinished())
		{
			if (lhsHead.getCurrent() != rhsHead.getCurrent())
				return false;

			lhsHead.goToNext();
			rhsHead.goToNext();
		}

		return lhsHead.isFinished() && rhsHead.isFinished();
	}

	bool areEqual(List& lhs, List& rhs)
	{
		if (lhs.getCount() != rhs.getCount())
			return false;
		else
		{
			IteratorPtr lhsHead(lhs.getHeadIterator());
			IteratorPtr rhsHead(rhs.getHeadIterator());

			return doIteratorsPointToEqualLists(*lhsHead, *rhsHead);
		}
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

			IteratorPtr iterator(list.getHeadIterator());
			Assert::IsTrue(iterator->isFinished());

			iterator.reset(list.getTailIterator());
			Assert::IsTrue(iterator->isFinished());
		}
		
		TEST_METHOD(InsertTailTest)
		{
			List list;
			list.insertAsTail(1);

			IteratorPtr tailIterator(list.getTailIterator());
			IteratorPtr headIterator(list.getHeadIterator());

			Assert::AreEqual(tailIterator->getCurrent(), headIterator->getCurrent());

			list.removeHead();

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				list.insertAsTail(i);
				Assert::IsTrue(list.getCount() == i + 1, L"Count is not set properly when inserting as tail");
			}

			headIterator.reset(list.getHeadIterator());

			int i = 0;
			for (; !headIterator->isFinished(); headIterator->goToNext())
			{
				Assert::IsTrue(headIterator->getCurrent() == i, L"Inserting as tail is not working properly");
				++i;
			}
		}

		TEST_METHOD(InsertHeadTest)
		{
			List list;
			list.insertAsHead(1);

			IteratorPtr tailIterator(list.getTailIterator());
			IteratorPtr headIterator(list.getHeadIterator());

			Assert::AreEqual(tailIterator->getCurrent(), headIterator->getCurrent());

			list.removeHead();

			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				list.insertAsHead(i);
				Assert::IsTrue(list.getCount() == i + 1, L"Count is not set properly when inserting as head");
			}

			headIterator.reset(list.getHeadIterator());

			int i = NUMBER_OF_ITEMS_TO_INSERT - 1;
			for (; !headIterator->isFinished(); headIterator->goToNext())
			{
				Assert::AreEqual(headIterator->getCurrent(), i, L"Adding as head is not working properly");
				--i;
			}
		}

		TEST_METHOD(AppendingToListCountTest)
		{
			List destination;
			fillListAddingTail(destination, NUMBER_OF_ITEMS_TO_INSERT);

			const int DEST_INITIAL_COUNT = destination.getCount();

			List source;
			destination.appendList(source);

			Assert::IsTrue(destination.getCount() == DEST_INITIAL_COUNT, L"Appending an empty list changes count");

			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT / 2);
			destination.appendList(source);

			Assert::IsTrue(destination.getCount() == (DEST_INITIAL_COUNT + source.getCount()), L"Appending a non-empty list changes count incorrectly");
		}
		
		TEST_METHOD(AppendingToListReconstruction)
		{
			List destination;
			List source;

			fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT);
			destination.appendList(source);

			Assert::IsTrue( areEqual(destination, source) );

			IteratorPtr destinationTail(destination.getTailIterator());
			destination.appendList(source);

			destinationTail->goToNext();
			Assert::IsFalse(destinationTail->isFinished(), L"The node after tail is null after appending non-empty list");

			IteratorPtr sourceHead(source.getHeadIterator());

			Assert::IsTrue(doIteratorsPointToEqualLists(*destinationTail, *sourceHead));
		}

		TEST_METHOD(RemovingHeadTest)
		{
			List list;
			fillListAddingHead(list, NUMBER_OF_ITEMS_TO_INSERT);

			IteratorPtr iterator(list.getHeadIterator());
			for (int i = NUMBER_OF_ITEMS_TO_INSERT; i > 0; --i)
			{
				Assert::IsTrue(iterator->getCurrent() == i - 1, L"Removing head does not manage head properly");
				Assert::IsTrue(list.getCount() == i, L"Removing head does not manage count properly");

				iterator->goToNext();
				list.removeHead();
			}
		}
		
		TEST_METHOD(RemovingTailTest)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			IteratorPtr iterator(list.getTailIterator());
			for (int i = NUMBER_OF_ITEMS_TO_INSERT; i > 0; --i)
			{
				Assert::IsTrue(iterator->getCurrent() == i - 1, L"Removing tail does not manage tail properly");
				Assert::IsTrue(list.getCount() == i, L"Removing tail does not manage count properly");

				list.removeTail();
				iterator.reset(list.getTailIterator()); //TODO : getTail() functions which returns const T&
			}
		}

		TEST_METHOD(InsertionAfterNullIterator)
		{
			List list;

			IteratorPtr iterator(nullptr);
			IteratorPtr tail(nullptr);
			for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
			{
				iterator.reset(list.getTailIterator());
				//forse null
				iterator->goToNext();

				//should insert it as tail
				list.insertAfter(*iterator, i);

				tail.reset(list.getTailIterator());
				Assert::IsTrue(tail->getCurrent() == i);  //TODO : getTail() functions which returns const T&
			}
		}

		//TEST_METHOD(InsertingAfterHeadIterator)
		//{
		//	List list;
		//	list.insertAsHead(1);

		//	for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
		//	{
		//		ListIterator headIterator = list.getHeadIterator();
		//		list.insertAfter(headIterator, i);

		//		++headIterator;

		//		Assert::IsTrue(headIterator, L"Insertion after iterator does not update its successor");
		//		Assert::IsTrue(*headIterator == i, L"Insertion after iterator does not construct with the value passed");
		//	}
		//}

		//TEST_METHOD(InsertingBetweenNodesWithInsertAfterIterator)
		//{
		//	List list;

		//	list.insertAsHead(1);
		//	list.insertAsTail(3);

		//	ListIterator headIterator = list.getHeadIterator();
		//	list.insertAfter(headIterator, 2);

		//	for (int i = 1; i < 4; ++i)
		//	{
		//		Assert::IsTrue(*headIterator == i, L"Inserting between two nodes with iterator is not working properly");
		//		++headIterator;
		//	}

		//	Assert::IsFalse(headIterator);
		//}

		//TEST_METHOD(InsertingAfterTailIterator)
		//{
		//	List list;
		//	list.insertAsTail(1);

		//	for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
		//	{
		//		ListIterator tailIterator = list.getTailIterator();
		//		list.insertAfter(tailIterator, i);

		//		++tailIterator;

		//		Assert::IsTrue(tailIterator);
		//		Assert::IsTrue(*tailIterator == i, L"Insertion after tail iterator does not construct with the value passed");
		//	}
		//}

		//TEST_METHOD(InsertionBeforeNullIterator)
		//{
		//	List list;

		//	for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
		//	{
		//		ListIterator iterator = list.getTailIterator();
		//		//forse null
		//		++iterator;

		//		//should insert it as head
		//		list.insertBefore(iterator, i);

		//		ListIterator headIterator = list.getHeadIterator();
		//		Assert::IsTrue(*headIterator == i);
		//	}
		//}

		//TEST_METHOD(InsertingBeforeHeadIterator)
		//{
		//	List list;
		//	list.insertAsHead(1);

		//	for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
		//	{
		//		ListIterator headIterator = list.getHeadIterator();
		//		list.insertBefore(headIterator, i);

		//		ListIterator newHead = list.getHeadIterator();
		//		Assert::IsTrue(*newHead == i);
		//	}
		//}

		//TEST_METHOD(InsertingBetweenNodesWithInsertBeforeIterator)
		//{
		//	List list;

		//	list.insertAsHead(1);
		//	list.insertAsTail(3);

		//	ListIterator tailIterator = list.getTailIterator();
		//	list.insertBefore(tailIterator, 2);

		//	ListIterator iterator = list.getHeadIterator();
		//	for (int i = 1; i < 4; ++i)
		//	{
		//		Assert::IsTrue(*iterator == i);
		//		++iterator;
		//	}

		//	Assert::IsFalse(iterator);
		//}

		//TEST_METHOD(RemovingAtNullIterator)
		//{
		//	List emptyList;

		//	ListIterator nullIterator = emptyList.getHeadIterator();
		//	emptyList.removeAt(nullIterator);

		//	Assert::IsTrue(emptyList.getCount() == 0);
		//	Assert::IsTrue(emptyList.isEmpty());
		//}

		//TEST_METHOD(RemovingAtHeadIterator)
		//{
		//	List list;
		//	fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

		//	for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
		//	{
		//		ListIterator headIterator = list.getHeadIterator();

		//		Assert::IsTrue(*headIterator == i, L"Removing at head iterator does not manage head properly");
		//		Assert::IsTrue(list.getCount() == NUMBER_OF_ITEMS_TO_INSERT - i, L"Removing at head iterator does not manage count properly");

		//		list.removeAt(headIterator);

		//		Assert::IsFalse(headIterator, L"Removing at head iterator does not null the iterator");
		//	}

		//	Assert::IsTrue(list.isEmpty());
		//}


		//TEST_METHOD(RemovingAtTailIterator)
		//{
		//	List list;
		//	fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

		//	for (int i = NUMBER_OF_ITEMS_TO_INSERT - 1; i >= 0; --i)
		//	{
		//		ListIterator tailIterator = list.getTailIterator();

		//		Assert::IsTrue(*tailIterator == i, L"Removing at tail iterator does not manage tail properly");
		//		Assert::IsTrue(list.getCount() == i + 1, L"Removing at tail iterator does not manage count properly");

		//		list.removeAt(tailIterator);

		//		Assert::IsFalse(tailIterator, L"Removing at iterator does not null iterator");
		//	}

		//	Assert::IsTrue(list.isEmpty());
		//}

		//TEST_METHOD(RemovingBetweenNodesWithIterator)
		//{
		//	List list;
		//	fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

		//	for (int i = 0; i < NUMBER_OF_ITEMS_TO_INSERT; ++i)
		//	{
		//		ListIterator secondNodeIterator = list.getHeadIterator();
		//		++secondNodeIterator;

		//		//head stays 0 after removing its successor
		//		Assert::IsTrue( *( list.getHeadIterator() ) == 0, L"Removing between nodes with iterator does not manage predecessor properly");
		//	
		//		//if at least two nodes in the list
		//		if (i < NUMBER_OF_ITEMS_TO_INSERT - 1)
		//		{
		//			Assert::IsTrue(*secondNodeIterator == i + 1, L"Removing between nodes with iterator does not manage successor properly");
		//		}

		//		Assert::IsTrue(list.getCount() == NUMBER_OF_ITEMS_TO_INSERT - i, L"Removing between nodes with iterator does not manage count properly");

		//		list.removeAt(secondNodeIterator);
		//		Assert::IsFalse(secondNodeIterator, L"Removing at iterator does not null iterator");
		//	}

		//	Assert::IsTrue(list.getCount() == 1);
		//	Assert::IsTrue( *( list.getHeadIterator() ) == 0 );
		//}


		//TEST_METHOD(CopyCtorTest)
		//{
		//	List source;

		//	List destinationOne(source);
		//	Assert::IsTrue(areEqual(source, destinationOne));

		//	fillListAddingHead(source, NUMBER_OF_ITEMS_TO_INSERT);

		//	List destinationTwo(source);
		//	Assert::IsTrue(areEqual(source, destinationTwo));
		//}

		//TEST_METHOD(MoveCtorFromEmpty)
		//{
		//	List source;
		//	List destination(std::move(source));

		//	Assert::IsTrue(destination.getCount() == 0);
		//	Assert::IsTrue(destination.isEmpty());

		//	Assert::IsTrue(source.getCount() == 0);
		//	Assert::IsTrue(source.isEmpty());
		//}

		//TEST_METHOD(MoveCtorFromNonEmpty)
		//{
		//	List source;
		//	fillListAddingTail(source, NUMBER_OF_ITEMS_TO_INSERT);

		//	List initialSourceCopy(source);

		//	List destination(std::move(source));

		//	Assert::IsTrue( areEqual(destination, initialSourceCopy) );

		//	Assert::IsTrue(source.getCount() == 0);
		//	Assert::IsTrue(source.isEmpty());
		//}

		//TEST_METHOD(CopyAssignmentEmptyToEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	lhs = rhs;

		//	Assert::IsTrue(areEqual(lhs, rhs));
		//}

		//TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	fillListAddingTail(rhs, NUMBER_OF_ITEMS_TO_INSERT);

		//	lhs = rhs;

		//	Assert::IsTrue(areEqual(lhs, rhs));
		//}

		//TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);
		//	fillListAddingTail(rhs, NUMBER_OF_ITEMS_TO_INSERT / 2);

		//	lhs = rhs;

		//	Assert::IsTrue(areEqual(lhs, rhs));
		//}

		//TEST_METHOD(CopyAssignmentEmptyToNonEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);

		//	lhs = rhs;

		//	Assert::IsTrue(areEqual(lhs, rhs));
		//}
		//
		//TEST_METHOD(MoveAssignmentEmptyToEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	lhs = std::move(rhs);

		//	Assert::IsTrue(lhs.getCount() == 0);
		//	Assert::IsTrue(lhs.isEmpty());
		//	Assert::IsTrue(rhs.getCount() == 0);
		//	Assert::IsTrue(rhs.isEmpty());
		//}

		//TEST_METHOD(MoveAssignmentNonEmptyToEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	fillListAddingHead(rhs, NUMBER_OF_ITEMS_TO_INSERT);

		//	List initialRhsCopy(rhs);

		//	lhs = std::move(rhs);

		//	Assert::IsTrue(areEqual(lhs, initialRhsCopy));
		//	Assert::IsTrue(rhs.getCount() == 0);
		//	Assert::IsTrue(rhs.isEmpty());
		//}
		//
		//TEST_METHOD(MoveAssignmentEmptyToNonEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);

		//	lhs = std::move(rhs);

		//	Assert::IsTrue(lhs.getCount() == 0);
		//	Assert::IsTrue(lhs.isEmpty());
		//	Assert::IsTrue(rhs.getCount() == 0);
		//	Assert::IsTrue(rhs.isEmpty());
		//}


		//TEST_METHOD(MoveAssignmentNonEmptyToNonEmpty)
		//{
		//	List lhs;
		//	List rhs;

		//	fillListAddingHead(lhs, NUMBER_OF_ITEMS_TO_INSERT);
		//	fillListAddingTail(rhs, NUMBER_OF_ITEMS_TO_INSERT / 2);

		//	List initialRhsCopy(rhs);

		//	lhs = std::move(rhs);

		//	Assert::IsTrue(areEqual(lhs, initialRhsCopy));
		//	Assert::IsTrue(rhs.getCount() == 0);
		//	Assert::IsTrue(rhs.isEmpty());
		//}
	};
}