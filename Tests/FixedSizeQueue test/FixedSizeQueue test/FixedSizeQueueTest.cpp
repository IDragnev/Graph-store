#include "CppUnitTest.h"
#include "..\..\..\Graph store\Containers\Fixed size queue\FixedSizeQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Containers::FixedSizeQueue;

namespace Queuetest
{		
	TEST_CLASS(FixedSizeQueueTest)
	{
	private:
		using Queue = FixedSizeQueue<int>;

		static bool containsExactly(Queue& queue, std::initializer_list<int> values)
		{
			for (auto i : values)
			{
				if (queue.isEmpty() || queue.dequeue() != i)
				{
					return false;
				}
			}

			return true;
		}

	public:
		TEST_METHOD(SizeConstructedQueueIsEmpty)
		{
			Queue queue(10);

			Assert::IsTrue(queue.isEmpty());
			Assert::IsFalse(queue.isFull());
		}
		TEST_METHOD(InitializerListCtor)
		{
			Queue queue{ 1, 2, 3 };

			Assert::IsTrue(queue.isFull(), L"The queue is not full");
			Assert::IsTrue(containsExactly(queue, {1, 2, 3}), L"The queue has invalid contents");
		}

		TEST_METHOD(Enqueue)
		{
			Queue queue(2);

			queue.enqueue(1);

			Assert::IsFalse(queue.isEmpty(), L"The queue is still empty");
			Assert::AreEqual(queue.peekHead(), 1, L"Incorrect item inserted");
		}

		TEST_METHOD(Dequeue)
		{
			Queue queue{ 1, 2 };
		
			auto oldHead = queue.dequeue();

			Assert::AreEqual(oldHead, 1, L"Dequeued item is incorrect");
			Assert::AreEqual(queue.peekHead(), 2, L"New head is incorrect");
		}

		TEST_METHOD(Empty)
		{
			Queue queue{ 1, 2, 3, 4, 5 };

			queue.empty();

			Assert::IsTrue(queue.isEmpty());
		}
	};
}