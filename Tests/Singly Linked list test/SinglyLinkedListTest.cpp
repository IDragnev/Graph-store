#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SinglyLinkedListTest
{
	TEST_CLASS(SinglyLinkedListTest)
	{
	private:
		typedef SinglyLinkedList<int>::Iterator ListIterator;
		typedef SinglyLinkedList<int>::ConstIterator ListConstIterator;
		typedef SinglyLinkedList<int> List;

	public:
		TEST_METHOD(testDefaultConstructedListIsEmpty)
		{
			List list;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testInitializerListCtor)
		{
			List list{ 0, 1, 2, 3, 4 };

			int i = 0;
			std::for_each(list.getBeginConstIterator(), list.getEndConstIterator(), 
				[&](int current)
			{
				Assert::AreEqual(current, i++);
			});		
		}

		TEST_METHOD(testEmptyListReturnsNullIterators)
		{
			List list;

			ListConstIterator head = list.getBeginConstIterator();
			ListConstIterator tail = list.getEndConstIterator();

			Assert::IsFalse(head, L"Begin iterator of empty list is not null");
			Assert::IsFalse(tail, L"End iterator of empty list is not null");
		}
	
		TEST_METHOD(testTailInsertionUpdatesCountAndTail)
		{
			List list;

			list.insertAsTail(0);

			Assert::AreEqual(list.getCount(), 1U, L"Count is not updated");
			Assert::AreEqual(list.getTail(), 0, L"Tail is not updated");
		}

		TEST_METHOD(testInsertionAsHeadUpdatesCountAndHead)
		{
			List list;

			list.insertAsHead(0);

			Assert::AreEqual(list.getCount(), 1U, L"Count is not updated");
			Assert::AreEqual(list.getHead(), 0, L"Head is not updated");
		}
		
		TEST_METHOD(testAppendListLValue)
		{
			List source{ 4, 5, 6 };
			List destination{ 1, 2, 3 };
			
			destination.appendList(source);

			Assert::IsTrue(destination == List{ 1, 2, 3, 4, 5, 6 });
		}

		TEST_METHOD(testAppendListRValue)
		{
			List source{ 4, 5, 6 };
			List destination{ 1, 2, 3 };

			destination.appendList(std::move(source));

			Assert::IsTrue(destination == List{ 1, 2, 3, 4, 5, 6 }, L"Appended-to list has invalid contents");
			Assert::IsTrue(source.isEmpty(), L"Move-appended list is not empty");
		}

		TEST_METHOD(testRemoveHeadUpdatesCountAndHead)
		{
			List list{ 1, 2, 3 };
			
			list.removeHead();

			Assert::AreEqual(list.getHead(), 2, L"Head is not updated");
			Assert::AreEqual(list.getCount(), 2U, L"Count is not updated");
		}
		
		TEST_METHOD(testRemoveTailUpdatesCoundAndTail)
		{
			List list{ 1, 2, 3 };

			list.removeTail();

			Assert::AreEqual(list.getTail(), 2, L"Tail is not updated");
			Assert::AreEqual(list.getCount(), 2U, L"Count is not updated");
		}

		TEST_METHOD(testInsertionAfterNullIteratorInsertsAsTail)
		{
			List list{ 10, 11, 12 };
			ListIterator iterator = list.getEndIterator();

			list.insertAfter(iterator, 1);

			Assert::AreEqual(list.getTail(), 1);
		}

		TEST_METHOD(testInsertionAfterValidIterator)
		{
			List list{ 1 };
			ListIterator iterator = list.getBeginIterator();

			list.insertAfter(iterator, 2);

			Assert::AreEqual(list.getTail(), 2);
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertAfterIterator)
		{
			List list{ 1, 3 };
			
			ListIterator iterator = list.getBeginIterator();
			list.insertAfter(iterator, 2);

			Assert::IsTrue(list == List{ 1, 2, 3 });
		}

		TEST_METHOD(testInsertionBeforeNullIteratorInsertsAsHead)
		{
			List list{ 10 };

			ListIterator iterator = list.getEndIterator();
			list.insertBefore(iterator, 1);

			Assert::AreEqual(list.getHead(), 1);
		}

		TEST_METHOD(testInsertionBeforeIterator)
		{
			List list{ 1 };

			ListIterator iterator = list.getBeginIterator();
			list.insertBefore(iterator, 20);

			Assert::AreEqual(list.getHead(), 20);
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertBeforeIterator)
		{
			List list{ 1,3 };
			ListIterator iterator = list.getBeginIterator();
			++iterator;

			list.insertBefore(iterator, 2);

			Assert::IsTrue(list == List{ 1, 2, 3 });
		}

		TEST_METHOD(testRemovalAtNullIteratorDoesNothing)
		{
			List emptyList;
			ListIterator nullIterator = emptyList.getEndIterator();

			emptyList.removeAt(nullIterator);

			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(testRemovalAtIterator)
		{
			List list{ 1, 2, 3 };
			ListIterator iterator = list.getBeginIterator();
			++iterator;

			list.removeAt(iterator);

			Assert::IsTrue(list == List{ 1, 3 });

		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			List source;
			List destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			List source{ 1, 2, 3 };
			List destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			List source;
			List destination{ std::move(source) };

			Assert::IsTrue(destination.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(source.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			List source{ 1, 2, 3, 4 };
			List destination{ std::move(source) };

			Assert::IsTrue(destination == List{1, 2, 3, 4}, L"Moved-in list has invalid contents");
			Assert::IsTrue(source.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			List lhs;
			List rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			List lhs;
			List rhs{ 1, 2, 3, 4 };

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			List lhs{ 1, 2, 3, 4 };
			List rhs{ 12, 13, 14, 15 };

			lhs = rhs;

			Assert::IsTrue(lhs ==  rhs);
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			List lhs{ 1, 2, 3, 4 };
			List rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}
		
		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			List lhs;
			List rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			List lhs;
			List rhs{ 1, 2, 3, 4 };

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == List{ 1, 2, 3, 4 }, L"Moved-in list has invalid contents");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}
		
		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			List lhs{ 1, 2, 3, 4 };
			List rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			List lhs{ 1, 2, 3, 4 };
			List rhs{ 10, 9, 8, 7 };

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == List{10, 9, 8, 7}, L"Moved-in list has invalid contents");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}
	};
}