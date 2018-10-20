#include "CppUnitTest.h"
#include "..\..\..\Graph store\Containers\Queue\Queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Containers::Queue;

namespace Queuetest
{		
	TEST_CLASS(QueueTest)
	{
	private:
		using Queue = Queue<int>;

	public:	
		TEST_METHOD(testDefaultCtorConstructsEmptyQueue)
		{
			auto queue = Queue{};

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testInitializerListCtor)
		{
			auto queue = Queue{ 1, 2, 3 };

			auto i = 1;
			while (!queue.isEmpty())
			{
				Assert::AreEqual(queue.dequeue(), i++);
			}
		}

		TEST_METHOD(testEnqueue)
		{
			auto queue = Queue{};

			queue.enqueue(1);

			Assert::IsFalse(queue.isEmpty(), L"The queue is empty after insertion");
			Assert::AreEqual(queue.peekHead(), 1, L"Invalid head");
		}

		TEST_METHOD(testDequeue)
		{
			auto queue = Queue{ 1, 2 };
		
			auto oldHead = queue.dequeue();

			Assert::AreEqual(oldHead, 1, L"Incorrect dequeued item");
			Assert::AreEqual(queue.peekHead(), 2, L"Incorrect new head item");
		}

		TEST_METHOD(testEmptyEmptiesTheQueue)
		{
			auto queue = Queue{ 1, 2, 3, 4, 5 };

			queue.empty();

			Assert::IsTrue(queue.isEmpty());
		}
	};
}