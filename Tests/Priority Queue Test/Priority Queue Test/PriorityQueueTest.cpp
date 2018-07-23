#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/Priority Queue/PriorityQueue.h"

#include "TestItem.h"

namespace PriorityQueueTest
{	
	class CompareFunction
	{
	public:
		bool operator()(int lhs, int rhs) { return lhs < rhs; }
	};

	class HandleSetter
	{
		void operator()(TestItem* itemPtr, PriorityQueueHandle handle) const
		{
			assert(itemPtr);
			itemPtr->setHandle(handle);
		}
	};

	TEST_CLASS(PriorityQueueTest)
	{
	private:
		typedef PriorityQueue<TestItem, int, CompareFunction, HandleSetter> MaxPriorityQueue;

	public:	
		TEST_METHOD(testDefaultConstructedQueueIsEmpty)
		{
			MaxPriorityQueue queue;

			Assert::IsTrue(queue.isEmpty());
		}
	};
}