#include "CppUnitTest.h"
#include "../../Graph store/Containers/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Containers::SinglyLinkedList;
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
		TEST_METHOD(DefaultConstructedListIsEmpty)
		{
			IntList list;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(IteratorToConstIteratorConversion)
		{
			IntList list;

			auto constBegin = cbegin(list);
			auto nonConstEnd = end(list);

			Assert::IsTrue(constBegin == nonConstEnd);
			Assert::IsFalse(constBegin != nonConstEnd);
		}
		TEST_METHOD(InitializerListCtor)
		{
			IntList list{ 0, 1, 2, 3, 4 };

			auto i = 0;
			for(auto current: list)
			{
				Assert::AreEqual(current, i++);
			}	
		}

		TEST_METHOD(RangeCtor)
		{
			IntList source{ 1, 2, 3, 4 };
			IntList destination(cbegin(source), cend(source));

			Assert::IsTrue(destination == source);
		}

		TEST_METHOD(RangeCtorWithMoveIterator)
		{
			StringList source{ "one", "two", "three" };
			StringList destination{ std::make_move_iterator(begin(source)),
									std::make_move_iterator(end(source)) };

			Assert::IsTrue(source == StringList{ "", "", "" }, L"Moved-from list has invalid content");
			Assert::IsTrue(destination == StringList{ "one", "two", "three" }, L"Moved-in list has invalid content");
		}

		TEST_METHOD(EmptyListReturnsInvalidIterators)
		{
			IntList list;

			Assert::IsFalse(cbegin(list), L"Begin iterator of empty list is not null");
			Assert::IsFalse(cend(list), L"End iterator of empty list is not null");
		}
	
		TEST_METHOD(InsertBack)
		{
			IntList list;

			list.insertBack(0);

			Assert::AreEqual(list.getCount(), 1U, L"Count is not updated");
			Assert::AreEqual(list.getBack(), 0, L"Tail is not updated");
		}

		TEST_METHOD(InsertFront)
		{
			IntList list;

			list.insertFront(0);

			Assert::AreEqual(list.getCount(), 1U, L"Count is not updated");
			Assert::AreEqual(list.getFront(), 0, L"Head is not updated");
		}
		
		TEST_METHOD(AppendList)
		{
			IntList source{ 4, 5, 6 };
			IntList destination{ 1, 2, 3 };
			
			destination.appendList(source);

			Assert::IsTrue(destination == IntList{ 1, 2, 3, 4, 5, 6 });
		}

		TEST_METHOD(RemoveFront)
		{
			IntList list{ 1, 2, 3 };
			
			list.removeFront();

			Assert::AreEqual(list.getFront(), 2, L"Head is not updated");
			Assert::AreEqual(list.getCount(), 2u, L"Count is not updated");
		}
		
		TEST_METHOD(RemoveBack)
		{
			IntList list{ 1, 2, 3 };

			list.removeBack();

			Assert::AreEqual(list.getBack(), 2, L"Tail is not updated");
			Assert::AreEqual(list.getCount(), 2U, L"Count is not updated");
		}

		TEST_METHOD(InsertionAfterNullIteratorInsertsBack)
		{
			IntList list{ 10, 11, 12 };

			list.insertAfter(end(list), 1);

			Assert::AreEqual(list.getBack(), 1);
		}

		TEST_METHOD(InsertionAfterValidIterator)
		{
			IntList list{ 1 };

			list.insertAfter(begin(list), 2);

			Assert::AreEqual(list.getBack(), 2);
		}

		TEST_METHOD(InsertionBetweenElementsWithInsertAfterIterator)
		{
			IntList list{ 1, 3 };
			
			list.insertAfter(begin(list), 2);

			Assert::IsTrue(list == IntList{ 1, 2, 3 });
		}

		TEST_METHOD(InsertionBeforeNullIteratorInsertsFront)
		{
			IntList list{ 10 };

			list.insertBefore(end(list), 1);

			Assert::AreEqual(list.getFront(), 1);
		}

		TEST_METHOD(InsertionBeforeValidIterator)
		{
			IntList list{ 1 };

			list.insertBefore(begin(list), 20);

			Assert::AreEqual(list.getFront(), 20);
		}

		TEST_METHOD(InsertionBetweenElementsWithInsertBeforeIterator)
		{
			IntList list{ 1, 3 };
			auto iterator = begin(list);
			++iterator;

			list.insertBefore(iterator, 2);

			Assert::IsTrue(list == IntList{ 1, 2, 3 });
		}

		TEST_METHOD(RemovingAtNullIteratorDoesNothing)
		{
			IntList emptyList;

			emptyList.removeAt(end(emptyList));

			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(RemovingAtValidIterator)
		{
			IntList list{ 1, 2, 3 };
			auto iterator = begin(list);
			++iterator;

			list.removeAt(iterator);

			Assert::IsTrue(list == IntList{ 1, 3 });
		}

		TEST_METHOD(CopyCtorFromEmptySource)
		{
			IntList source;
			IntList destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(CopyCtorFromNonEmptySource)
		{
			IntList source{ 1, 2, 3 };
			IntList destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(MoveCtorFromEmptySource)
		{
			IntList source;
			IntList destination{ std::move(source) };

			Assert::IsTrue(destination.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(source.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(MoveCtorFromNonEmptySource)
		{
			IntList source{ 1, 2, 3, 4 };
			IntList destination{ std::move(source) };

			Assert::IsTrue(destination == IntList{1, 2, 3, 4}, L"Moved-in list has invalid contents");
			Assert::IsTrue(source.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(CopyAssignmentEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs{ 1, 2, 3, 4 };

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs{ 12, 13, 14, 15 };

			lhs = rhs;

			Assert::IsTrue(lhs ==  rhs);
		}

		TEST_METHOD(CopyAssignmentEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}
		
		TEST_METHOD(MoveAssignmentEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(MoveAssignmentNonEmptyToEmpty)
		{
			IntList lhs;
			IntList rhs{ 1, 2, 3, 4 };

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == IntList{ 1, 2, 3, 4 }, L"Moved-in list has invalid contents");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}
		
		TEST_METHOD(MoveAssignmentEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs.isEmpty(), L"Moved-in list is not empty");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}

		TEST_METHOD(MoveAssignmentNonEmptyToNonEmpty)
		{
			IntList lhs{ 1, 2, 3, 4 };
			IntList rhs{ 10, 9, 8, 7 };

			lhs = std::move(rhs);

			Assert::IsTrue(lhs == IntList{10, 9, 8, 7}, L"Moved-in list has invalid contents");
			Assert::IsTrue(rhs.isEmpty(), L"Moved-from list is not empty");
		}
	};
}