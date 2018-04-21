#include "CppUnitTest.h"

#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef SinglyLinkedListIterator<int> ListIterator;
typedef SinglyLinkedList<int> List;
typedef std::unique_ptr<ListIterator> IteratorPtr;


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
			
			IteratorPtr iterator(list.getHeadIterator());

			int count = 0;
			while (!iterator->isFinished())
			{
				iterator->goToNext();
				++count;
			}

			Assert::IsTrue(count == NUMBER_OF_ITEMS_TO_INSERT);
		}

		TEST_METHOD(IteratorDerrefTest)
		{
			List list;
			fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

			IteratorPtr iterator(list.getHeadIterator());
			int i = 0;
			for (; !iterator->isFinished(); iterator->goToNext())
			{
				Assert::AreEqual(iterator->getCurrent(), i);
				++i;
			}
		}

		TEST_METHOD(IteratorEqualtyOperatorsTest)
		{
				List list;
				fillListAddingTail(list, NUMBER_OF_ITEMS_TO_INSERT);

				IteratorPtr head(list.getHeadIterator());
				IteratorPtr tail(list.getTailIterator());

				Assert::IsFalse(*head == *tail);
				Assert::IsTrue(*head != *tail);

				tail.reset(list.getHeadIterator());

				Assert::IsTrue(*head == *tail);
				Assert::IsFalse(*head != *tail);

				list.empty();

				head.reset(list.getHeadIterator());
				tail.reset(list.getTailIterator());

				Assert::IsTrue(*head == *tail);
				Assert::IsFalse(*head != *tail);
		}
	};
}