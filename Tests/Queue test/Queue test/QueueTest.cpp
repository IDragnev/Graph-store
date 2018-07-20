#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/Queue/Queue.h"

namespace Queuetest
{		
	TEST_CLASS(QueueTest)
	{
	private:
		typedef Queue<int> Queue;

	public:	
		TEST_METHOD(testCtorConstructsEmptyQueue)
		{
			Queue queue;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testEnqueue)
		{
			Queue queue;

			queue.enqueue(1);

			Assert::IsFalse(queue.isEmpty(), L"The queue is empty after enqueueing an item");
			Assert::AreEqual(queue.peekHead(), 1, L"enqueue is passed one item but enqueues another");
		}

		TEST_METHOD(testDequeue)
		{
			Queue queue;
			queue.enqueue(1);
			queue.enqueue(2);

			const int oldHead = queue.dequeue();

			Assert::AreEqual(oldHead, 1, L"Dequeued item is not the first enqueued item");
			Assert::AreEqual(queue.peekHead(), 2, L"The head of the queue is not the second element after calling dequeue");
		}

		TEST_METHOD(testEmptyEmptiesTheQueue)
		{
			Queue queue;
			queue.enqueue(1);
			queue.enqueue(0);

			queue.empty();

			Assert::IsTrue(queue.isEmpty());
		}
	};
}