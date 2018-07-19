#include "CppUnitTest.h"
#include "../../Graph store/Singly Linked List/SinglyLinkedList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef SinglyLinkedList<int>::Iterator ListIterator;
typedef SinglyLinkedList<int>::ConstIterator ListConstIterator;
typedef SinglyLinkedList<int> List;

namespace SinglyLinkedListTest
{		
	void fillListWithIntegersFromZeroTo(List& list, const int integer);
	void fillListWithIntegersFromZeroToInReverse(List& list, const int integer);

	TEST_CLASS(IteratorTest)
	{
	private:
		static const int BIG_INTEGER = 50;

	public:
		TEST_METHOD(testIteratorIteratesTheWholeList)
		{
			List list;
			fillListWithIntegersFromZeroTo(list, BIG_INTEGER - 1);

			ListConstIterator iterator = list.getHeadConstIterator();
			int count = 0;

			for (; iterator; ++iterator)
			{
				++count;
			}

			Assert::AreEqual(count, BIG_INTEGER);
		}

		TEST_METHOD(testIteratorReturnesTheCorrectValue)
		{
			List list;
			fillListWithIntegersFromZeroTo(list, BIG_INTEGER - 1);

			ListConstIterator iterator = list.getHeadConstIterator();
			int i = 0;

			for (; iterator; ++iterator)
			{
				Assert::AreEqual(*iterator, i);
				++i;
			}
		}
	};
}