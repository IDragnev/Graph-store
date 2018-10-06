#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::begin;
using std::end;
using std::cbegin;
using std::cend;

namespace SinglyLinkedListTest
{
	TEST_CLASS(SinglyLinkedListTest)
	{
	private:
		using IntList = SinglyLinkedList<int>;
		using StringList = SinglyLinkedList<std::string>;

	public:
		TEST_METHOD(testDefaultConstructedListIsEmpty)
		{
			IntList list;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testInitializerListCtor)
		{
			IntList list{ 0, 1, 2, 3, 4 };

			auto i = 0;
			for(auto&& current: list)
			{
				Assert::AreEqual(current, i++);
			}	
		}

		TEST_METHOD(testRangeCtor)
		{
			IntList source{ 1, 2, 3, 4 };
			IntList destination(cbegin(source), cend(source));

			Assert::IsTrue(destination == source);
		}

		TEST_METHOD(testRangeCtorWithMoveIterator)
		{
			StringList source{ "one", "two", "three" };
			StringList destination{ std::make_move_iterator(begin(source)),
									std::make_move_iterator(end(source)) };

			Assert::IsTrue(source == StringList{ "", "", "" }, L"Moved-from list has invalid content");
			Assert::IsTrue(destination == StringList{ "one", "two", "three" }, L"Moved-in list has invalid content");
		}

		TEST_METHOD(testEmptyListReturnsInvalidIterators)
		{
			IntList list;

			Assert::IsFalse(cbegin(list), L"Begin iterator of empty list is not null");
			Assert::IsFalse(cend(list), L"End iterator of empty list is not null");
		}
	
		TEST_METHOD(testTailInsertionUpdatesCountAndTail)
		{
			IntList list;

			list.insertAsTail(0);

			Assert::AreEqual(list.getCount(), 1U, L"Count is not updated");
			Assert::AreEqual(list.getTail(), 0, L"Tail is not updated");
		}

		TEST_METHOD(testInsertionAsHeadUpdatesCountAndHead)
		{
			IntList list;

			list.insertAsHead(0);

			Assert::AreEqual(list.getCount(), 1U, L"Count is not updated");
			Assert::AreEqual(list.getHead(), 0, L"Head is not updated");
		}
		
		TEST_METHOD(testAppendListLValue)
		{
			IntList source{ 4, 5, 6 };
			IntList destination{ 1, 2, 3 };
			
			destination.appendList(source);

			Assert::IsTrue(destination == IntList{ 1, 2, 3, 4, 5, 6 });
		}

		TEST_METHOD(testAppendListRValue)
		{
			IntList source{ 4, 5, 6 };
			IntList destination{ 1, 2, 3 };

			destination.appendList(std::move(source));

			Assert::IsTrue(destination == IntList{ 1, 2, 3, 4, 5, 6 }, L"Appended-to list has invalid contents");
			Assert::IsTrue(source.isEmpty(), L"Move-appended list is not empty");
		}

		TEST_METHOD(testRemoveHeadUpdatesCountAndHead)
		{
			IntList list{ 1, 2, 3 };
			
			list.removeHead();

			Assert::AreEqual(list.getHead(), 2, L"Head is not updated");
			Assert::AreEqual(list.getCount(), 2U, L"Count is not updated");
		}
		
		TEST_METHOD(testRemoveTailUpdatesCoundAndTail)
		{
			IntList list{ 1, 2, 3 };

			list.removeTail();

			Assert::AreEqual(list.getTail(), 2, L"Tail is not updated");
			Assert::AreEqual(list.getCount(), 2U, L"Count is not updated");
		}

		TEST_METHOD(testInsertionAfterNullIteratorInsertsAsTail)
		{
			IntList list{ 10, 11, 12 };

			list.insertAfter(end(list), 1);

			Assert::AreEqual(list.getTail(), 1);
		}

		TEST_METHOD(testInsertionAfterValidIterator)
		{
			IntList list{ 1 };

			list.insertAfter(begin(list), 2);

			Assert::AreEqual(list.getTail(), 2);
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertAfterIterator)
		{
			IntList list{ 1, 3 };
			
			list.insertAfter(begin(list), 2);

			Assert::IsTrue(list == IntList{ 1, 2, 3 });
		}

		TEST_METHOD(testInsertionBeforeNullIteratorInsertsAsHead)
		{
			IntList list{ 10 };

			list.insertBefore(end(list), 1);

			Assert::AreEqual(list.getHead(), 1);
		}

		TEST_METHOD(testInsertionBeforeIterator)
		{
			IntList list{ 1 };

			list.insertBefore(begin(list), 20);

			Assert::AreEqual(list.getHead(), 20);
		}

		TEST_METHOD(testInsertionBetweenElementsWithInsertBeforeIterator)
		{
			IntList list{ 1, 3 };
			auto iterator = begin(list);
			++iterator;

			list.insertBefore(iterator, 2);

			Assert::IsTrue(list == IntList{ 1, 2, 3 });
		}

		TEST_METHOD(testRemovalAtNullIteratorDoesNothing)
		{
			IntList emptyList;

			emptyList.removeAt(end(emptyList));

			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(testRemovalAtIterator)
		{
			IntList list{ 1, 2, 3 };
			auto iterator = begin(list);
			++iterator;

			list.removeAt(iterator);

			Assert::IsTrue(list == IntList{ 1, 3 });

		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			IntList source;
			IntList destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			IntList source{ 1, 2, 3 };
			IntList destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			IntList source;
			IntList destination{ std::move(source) };

			Assert::IsTrue(destination.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(source.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			IntList source{ 1, 2, 3, 4 };
			IntList destination{ std::move(source) };

			Assert::IsTrue(destination == IntList{1, 2, 3, 4}, L"Moved-in list has invalid contents");
			Assert::IsTrue(source.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs{ 1, 2, 3, 4 };

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs{ 12, 13, 14, 15 };

			lhs = rhs;

			Assert::IsTrue(lhs ==  rhs);
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}
		
		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs{ 1, 2, 3, 4 };

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == IntList{ 1, 2, 3, 4 }, L"Moved-in list has invalid contents");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}
		
		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs{ 10, 9, 8, 7 };

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == IntList{10, 9, 8, 7}, L"Moved-in list has invalid contents");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}
	};
}