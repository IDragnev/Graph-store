#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../Graph store/Dynamic Array/DArray.h"

namespace DArraytest
{		
	void fillArray(DArray<int>& arr, int count, int first = 0, int step = 1)
	{
		for (int i = first; i < count; i += step)
			arr.insert(i);
	}

	bool areEqual(const DArray<int>& lhs, const DArray<int>& rhs)
	{
		if (lhs.getSize() != rhs.getSize()) 
			return false;

		int count = lhs.getCount();

		if (count != rhs.getCount())
			return false;

		for (int i = 0; i < count; ++i)
		{
			if (lhs[i] != rhs[i])
				return false;
		}

		return true;
	}


	bool areSizeAndCountZero(const DArray<int>& dArray)
	{
		return dArray.getCount() == 0 && dArray.getSize() == 0;
	}


	TEST_CLASS(DArrayTest)
	{
	private:
		static const size_t INITIAL_SIZE = 50;

	public:
		
		TEST_METHOD(ConstructorTest)
		{
			for (size_t size = 0; size < INITIAL_SIZE; ++size)
			{
				for (size_t count = 0; count <= size; ++count)
				{
					DArray<int> dArray(size, count);
					Assert::IsTrue(dArray.getCount() == count);
					Assert::IsTrue(dArray.getSize() == size);
				}
			}
		}

		TEST_METHOD(GlobalSizeModifyingFunctions)
		{
			DArray<int> dArray(INITIAL_SIZE);
			fillArray(dArray, INITIAL_SIZE + 1);

			Assert::IsTrue(dArray.getSize() > INITIAL_SIZE);
			Assert::IsTrue(dArray.getCount() == INITIAL_SIZE + 1);

			dArray.empty();
			Assert::IsTrue(dArray.isEmpty());
			Assert::IsTrue(dArray.getSize() == 0);

			dArray.ensureSize(INITIAL_SIZE + 30);
			Assert::IsTrue(dArray.getSize() == (INITIAL_SIZE + 30));

			dArray.shrink(INITIAL_SIZE);
			Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
		}


		TEST_METHOD(CopyCtorFromEmptyArgument)
		{
			DArray<int> source;
			DArray<int> destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}


		TEST_METHOD(CopyCtorFromNonEmptyArgument)
		{
			DArray<int> source(INITIAL_SIZE);
			fillArray(source, INITIAL_SIZE + 10);

			DArray<int> destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}


		TEST_METHOD(MoveConstructorFromEmpty)
		{
			DArray<int> source;
			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));

		}


		TEST_METHOD(MoveConstructorFromNonEmpty)
		{
			DArray<int> source(INITIAL_SIZE);

			const size_t insertedItems = INITIAL_SIZE - 5;
			fillArray(source, insertedItems);

			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == insertedItems);
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE);
			Assert::IsFalse(destination.isEmpty());

			for (int i = 0; i < insertedItems; ++i)
				Assert::IsTrue(destination[i] == i);

		}


		TEST_METHOD(CopyAssignmentEmptyToEmpty)
		{
			DArray<int> source;
			DArray<int> dest;

			dest = source;

			Assert::IsTrue(areSizeAndCountZero(dest));
		}


		TEST_METHOD(CopyAssignmentEmptyToNonEmpty)
		{
			DArray<int> destination;
			fillArray(destination, INITIAL_SIZE);

			DArray<int> source;

			destination = source;

			Assert::IsTrue(areSizeAndCountZero(destination));
		}


		TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		{
			DArray<int> source(INITIAL_SIZE);
			fillArray(source, INITIAL_SIZE);

			DArray<int> destination;
			destination = source;

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> source(INITIAL_SIZE);
			fillArray(source, INITIAL_SIZE);

			DArray<int> destination;
			fillArray(destination, INITIAL_SIZE - 5, 0, 5);

			destination = source;

			Assert::IsTrue(areEqual(source, destination));
		}


		TEST_METHOD(MoveAssignmentEmptyToEmpty)
		{
			DArray<int> source;
			DArray<int> destination;

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));
		}


		TEST_METHOD(MoveAssignmentEmptyToNonEmpty)
		{
			DArray<int> destination;
			fillArray(destination, INITIAL_SIZE);

			DArray<int> source;

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));
		}

		TEST_METHOD(MoveAssignmentNonEmptyToEmpty)
		{
			DArray<int> source(INITIAL_SIZE);

			const size_t insertedItemsCount = INITIAL_SIZE - 5;
			fillArray(source, insertedItemsCount);

			DArray<int> destination;
			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == insertedItemsCount);
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE);
			Assert::IsFalse(destination.isEmpty());

			for (int i = 0; i < insertedItemsCount; ++i)
				Assert::IsTrue(destination[i] == i);
		}

		TEST_METHOD(MoveAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> source(INITIAL_SIZE);

			const size_t insertedItemsToSourceCount = INITIAL_SIZE - 5;
			fillArray(source, insertedItemsToSourceCount);

			DArray<int> destination;
			fillArray(destination, INITIAL_SIZE, 0, 5);

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == insertedItemsToSourceCount);
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE);
			Assert::IsFalse(destination.isEmpty());

			for (int i = 0; i < insertedItemsToSourceCount; ++i)
				Assert::IsTrue(destination[i] == i);
		}


		TEST_METHOD(ArrayInsertionTest)
		{
			DArray<int> destination(INITIAL_SIZE);
	
			DArray<int> source(INITIAL_SIZE);
			const size_t insertedItemsToSourceCount = INITIAL_SIZE - 5;
			fillArray(source, insertedItemsToSourceCount);

			destination.insert(source);

			Assert::IsTrue(destination.getCount() == source.getCount());
		
			for (int i = 0; i < insertedItemsToSourceCount; ++i)
				Assert::IsTrue(destination[i] == i);
		}


		TEST_METHOD(AddingAndRemoval)
		{
			DArray<int> dArray(INITIAL_SIZE);

			for (int i = 1; i < INITIAL_SIZE; ++i)
			{
				dArray.insert(i);
				Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
				Assert::IsTrue(dArray.getCount() == i);
				Assert::IsFalse(dArray.isEmpty());
				Assert::IsTrue(dArray[i - 1] == i);
			}

			int count = dArray.getCount();

			for (int i = 1; i < INITIAL_SIZE; ++i)
			{
				dArray.remove(0);
				Assert::IsTrue(dArray.getCount() == --count);
			}

			Assert::IsTrue(dArray.isEmpty());
		}


		TEST_METHOD(InsertAtTest)
		{
			DArray<int> dArray(INITIAL_SIZE);

			//inserting back
			for (int i = 0; i < INITIAL_SIZE; ++i)
			{
				dArray.insertAt(i, i);
				Assert::IsTrue(dArray.getCount() == i + 1);
				Assert::IsTrue(dArray[i] == i);
			}

			int count = dArray.getCount();

			//inserting between the elements
			for (int i = 0; i < INITIAL_SIZE; ++i)
			{
				for (int position = count; position >= 0; --position)
				{
					dArray.insertAt(position, i);
					Assert::IsTrue(dArray[position] == i);
				}
			}
		}
	};
}