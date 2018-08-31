#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SinglyLinkedListTest
{
	TEST_CLASS(SinglyLinkedListTest)
	{
	private:
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

			auto i = 0;
			for(auto&& current: list)
			{
				Assert::AreEqual(current, i++);
			}	
		}

		TEST_METHOD(testEmptyListReturnsInvalidIterators)
		{
			List list;

			Assert::IsFalse(cbegin(list), L"Begin iterator of empty list is not null");
			Assert::IsFalse(cend(list), L"End iterator of empty list is not null");
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

			list.insertAfter(end(list), 1);

			Assert::AreEqual(list.getTail(), 1);
		}

		TEST_METHOD(testInsertionAfterValidIterator)
		{
			List list{ 1 };

			list.insertAfter(begin(list), 2);

			Assert::AreEqual(list.getTail(), 2);
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertAfterIterator)
		{
			List list{ 1, 3 };
			
			list.insertAfter(begin(list), 2);

			Assert::IsTrue(list == List{ 1, 2, 3 });
		}

		TEST_METHOD(testInsertionBeforeNullIteratorInsertsAsHead)
		{
			List list{ 10 };

			list.insertBefore(end(list), 1);

			Assert::AreEqual(list.getHead(), 1);
		}

		TEST_METHOD(testInsertionBeforeIterator)
		{
			List list{ 1 };

			list.insertBefore(begin(list), 20);

			Assert::AreEqual(list.getHead(), 20);
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertBeforeIterator)
		{
			List list{ 1, 3 };
			auto iterator = begin(list);
			++iterator;

			list.insertBefore(iterator, 2);

			Assert::IsTrue(list == List{ 1, 2, 3 });
		}

		TEST_METHOD(testRemovalAtNullIteratorDoesNothing)
		{
			List emptyList;

			emptyList.removeAt(end(emptyList));

			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(testRemovalAtIterator)
		{
			List list{ 1, 2, 3 };
			auto iterator = begin(list);
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