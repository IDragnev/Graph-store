#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Graph store/Dynamic Array/DArray.h"

namespace DArraytest
{		
	void fillArray(DArray<int>& arr, int count, int first = 0, int step = 1)
	{
		for (int i = first; i < count; i += step)
			arr.insert(i);
	}

	bool areEqual(DArray<int>& lhs, DArray<int>& rhs)
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


	bool areSizeAndCountZero(DArray<int>& dArray)
	{
		return dArray.getCount() == 0 && dArray.getSize() == 0;
	}


	TEST_CLASS(DArrayTest)
	{
	public:
		
		TEST_METHOD(ConstructorTest)
		{
			for (size_t size = 0; size < 50; ++size)
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
			DArray<int> dArray(16);
			fillArray(dArray, 17);

			Assert::IsTrue(dArray.getSize() > 16);
			Assert::IsTrue(dArray.getCount() == 17);

			dArray.empty();
			Assert::IsTrue(dArray.isEmpty());
			Assert::IsTrue(dArray.getSize() == 0);

			dArray.ensureSize(64);
			Assert::IsTrue(dArray.getSize() == 64);

			dArray.shrink(32);
			Assert::IsTrue(dArray.getSize() == 32);
		}


		TEST_METHOD(CopyCtorFromEmptyArgument)
		{
			DArray<int> source;
			DArray<int> destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}



		TEST_METHOD(CopyCtorFromNonEmptyArgument)
		{
			DArray<int> source(16);
			fillArray(source, 10);

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
			DArray<int> source(35);
			fillArray(source, 30);

			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == 30);
			Assert::IsTrue(destination.getSize() == 35);
			Assert::IsFalse(destination.isEmpty());

			for (int i = 0; i < 30; ++i)
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
			fillArray(destination, 20);

			DArray<int> source;

			destination = source;

			Assert::IsTrue(areSizeAndCountZero(destination));
		}


		TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		{
			DArray<int> source(30);
			fillArray(source, 27);

			DArray<int> destination;
			destination = source;

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> source(30);
			fillArray(source, 27);

			DArray<int> destination;
			fillArray(destination, 20, 0, 5);

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
			fillArray(destination, 40);

			DArray<int> source;

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));
		}

		TEST_METHOD(MoveAssignmentNonEmptyToEmpty)
		{
			DArray<int> source(30);
			fillArray(source, 27);

			DArray<int> destination;
			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == 27);
			Assert::IsTrue(destination.getSize() == 30);
			Assert::IsFalse(destination.isEmpty());

			for (int i = 0; i < 27; ++i)
				Assert::IsTrue(destination[i] == i);
		}

		TEST_METHOD(MoveAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> source(30);
			fillArray(source, 27);

			DArray<int> destination;
			fillArray(destination, 7, 0, 5);

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == 27);
			Assert::IsTrue(destination.getSize() == 30);
			Assert::IsFalse(destination.isEmpty());

			for (int i = 0; i < 27; ++i)
				Assert::IsTrue(destination[i] == i);
		}


		TEST_METHOD(ArrayInsertionTest)
		{
			DArray<int> destination(8);
			fillArray(destination, 5);

			DArray<int> source(10);
			fillArray(source, 10, 5, 1);

			destination.insert(source);

			Assert::IsTrue(destination.getCount() == 5 + source.getCount());
		
			for (int i = 0; i < 10; ++i)
				Assert::IsTrue(destination[i] == i);
		}


		TEST_METHOD(AddingAndRemoval)
		{
			DArray<int> dArray(16);

			for (int i = 1; i < 11; ++i)
			{
				dArray.insert(i);
				Assert::IsTrue(dArray.getSize() == 16);
				Assert::IsTrue(dArray.getCount() == i);
				Assert::IsFalse(dArray.isEmpty());
				Assert::IsTrue(dArray[i - 1] == i);
			}

			int count = dArray.getCount();

			for (int i = 1; i < 11; ++i)
			{
				dArray.remove(0);
				Assert::IsTrue(dArray.getCount() == --count);
			}

			Assert::IsTrue(dArray.isEmpty());
		}


		TEST_METHOD(InsertAtTest)
		{
			DArray<int> dArray(16);

			//inserting back
			for (int i = 0; i < 16; ++i)
			{
				dArray.insertAt(i, i);
				Assert::IsTrue(dArray.getCount() == i + 1);
				Assert::IsTrue(dArray[i] == i);
			}

			int count = dArray.getCount();

			//inserting between the elements
			for (int i = 0; i < 10; ++i)
			{
				for (int j = count; j >= 0; --j)
				{
					dArray.insertAt(j, i);
					Assert::IsTrue(dArray[j] == i);
				}
			}
		}
	};
}