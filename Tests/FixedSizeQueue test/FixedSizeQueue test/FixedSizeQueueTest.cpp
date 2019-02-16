#include "CppUnitTest.h"
#include "..\..\..\Graph store\Containers\Fixed size queue\FixedSizeQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::Containers::FixedSizeQueue;
using IDragnev::Containers::DArray;
using std::literals::string_literals::operator""s;
using std::make_move_iterator;
using std::begin;
using std::end;

namespace Queuetest
{		
	TEST_CLASS(FixedSizeQueueTest)
	{
	private:
		using IntQueue = FixedSizeQueue<int>;
		using StringQueue = FixedSizeQueue<std::string>;
		using StringArray = DArray<std::string>;

		template <typename T>
		static bool containsExactly(FixedSizeQueue<T>& queue, std::initializer_list<T> values)
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
			IntQueue queue(10);

			Assert::IsTrue(queue.isEmpty());
			Assert::IsFalse(queue.isFull());
		}
		TEST_METHOD(InitializerListCtor)
		{
			IntQueue queue{ 1, 2, 3 };

			Assert::IsTrue(queue.isFull(), L"The queue is not full");
			Assert::IsTrue(containsExactly(queue, {1, 2, 3}), L"The queue has invalid contents");
		}

		TEST_METHOD(RangeCtor)
		{
			auto names = StringArray{ "Alex", "Sam", "John" };
			auto first = make_move_iterator(begin(names));
			auto last = make_move_iterator(end(names));

			StringQueue queue(first, last);

			Assert::IsTrue(containsExactly(queue, { "Alex"s, "Sam"s, "John"s }));
			Assert::IsTrue(names == StringArray{ "", "", "" });
		}
		TEST_METHOD(Enqueue)
		{
			IntQueue queue(2);

			queue.enqueue(1);

			Assert::IsFalse(queue.isEmpty(), L"The queue is still empty");
			Assert::AreEqual(queue.peekHead(), 1, L"Incorrect item inserted");
		}

		TEST_METHOD(Dequeue)
		{
			IntQueue queue{ 1, 2 };
		
			auto oldHead = queue.dequeue();

			Assert::AreEqual(oldHead, 1, L"Dequeued item is incorrect");
			Assert::AreEqual(queue.peekHead(), 2, L"New head is incorrect");
		}

		TEST_METHOD(Empty)
		{
			IntQueue queue{ 1, 2, 3, 4, 5 };

			queue.empty();

			Assert::IsTrue(queue.isEmpty());
		}
	};
}