#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef SinglyLinkedListIterator<int> ListIterator;
typedef SinglyLinkedList<int> List;

namespace SinglyLinkedListTest
{		

	void fillListAddingTail(List& list, int count);
	void fillListAddingHead(List& list, int count);

	TEST_CLASS(IteratorTest)
	{
	private:
		static const int NUMBER_OF_ITEMS_TO_INSERT = 50;

	public:
		TEST_METHOD(IteratorBoundariesTest)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			ListIterator iterator = list.getHeadIterator();
			int count = 0;

			for (; iterator; ++iterator)
				++count;

			Assert::IsTrue(count == NUMBER_OF_ITEMS_TO_INSERT);
		}

		TEST_METHOD(IteratorDerrefTest)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			ListIterator iterator = list.getHeadIterator();

			int i = 0;
			for (; iterator; ++iterator)
			{
				Assert::AreEqual(*iterator, i);
				++i;
			}
		}

		TEST_METHOD(IteratorEqualtyOperatorsTest)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			ListIterator head = list.getHeadIterator();
			ListIterator tail = list.getTailIterator();

			Assert::IsFalse(head == tail);
			Assert::IsTrue(head != tail);

			tail = list.getHeadIterator();

			Assert::IsTrue(head == tail);
			Assert::IsFalse(head != tail);

			list.empty();

			head = list.getHeadIterator();
			tail = list.getTailIterator();

			Assert::IsTrue(head == tail);
			Assert::IsFalse(head != tail);
		}
	};
}