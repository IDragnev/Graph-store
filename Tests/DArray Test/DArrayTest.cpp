#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../Graph store/Dynamic Array/DArray.h"

namespace DArraytest
{		
	void fillArrayWithIntegersFromZeroToWithStep(DArray<int>& arr, int biggest, int step)
	{
		for (int i = 0; i <= biggest; i += step)
		{
			arr.insert(i);
		}
	}

	bool areEqual(const DArray<int>& lhs, const DArray<int>& rhs)
	{
		if (lhs.getSize() != rhs.getSize())
		{
			return false;
		}

		const int count = lhs.getCount();

		if (count != rhs.getCount())
		{
			return false;
		}

		for (int i = 0; i < count; ++i)
		{
			if (lhs[i] != rhs[i])
			{
				return false;
			}
		}

		return true;
	}


	bool areSizeAndCountZero(const DArray<int>& dArray)
	{
		return dArray.getCount() == 0 && dArray.getSize() == 0;
	}

	bool containsExactlyTheIntegersFromZeroTo(const DArray<int>& dArray, int biggest)
	{
		if (dArray.getCount() != biggest + 1)
		{
			return false;
		}

		for (int i = 0; i <= biggest; ++i)
		{
			if (dArray[i] != i)
			{
				return false;
			}
		}

		return true;
	}

	TEST_CLASS(DArrayTest)
	{
	private:
		static const size_t INITIAL_SIZE = 50;

	public:
		TEST_METHOD(testEnsureSizeEnlargesWhenGivenSizeIsGreater)
		{
			DArray<int> dArray;

			dArray.ensureSize(INITIAL_SIZE);
			Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
		}

		TEST_METHOD(testEnsureSizeDoesNotDoAnythingWhenGivenSizeIsNotGreater)
		{
			DArray<int> dArray(INITIAL_SIZE);

			for (int i = 0; i < INITIAL_SIZE; ++i)
			{
				dArray.ensureSize(1);
				Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
			}
		}

		TEST_METHOD(testShrinkSizeActuallyShrinksTheSize)
		{
			DArray<int> dArray(INITIAL_SIZE + 15);

			dArray.shrink(INITIAL_SIZE);
			Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
		}

		TEST_METHOD(testEmptyLeavesSizeZero)
		{
			DArray<int> dArray(INITIAL_SIZE);
			fillArrayWithIntegersFromZeroToWithStep(dArray, INITIAL_SIZE, 1);

			dArray.empty();
			Assert::IsTrue(dArray.isEmpty());
			Assert::IsTrue(dArray.getSize() == 0);
		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			DArray<int> source;
			DArray<int> destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			DArray<int> source(INITIAL_SIZE);
			fillArrayWithIntegersFromZeroToWithStep(source, INITIAL_SIZE + 10, 1);

			DArray<int> destination(source);

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			DArray<int> source;
			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			DArray<int> source(INITIAL_SIZE);

			const size_t greatestInteger = INITIAL_SIZE - 5;
			fillArrayWithIntegersFromZeroToWithStep(source, greatestInteger, 1);

			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source));

			Assert::IsTrue(destination.getCount() == greatestInteger + 1);
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE);
			Assert::IsFalse(destination.isEmpty());

			Assert::IsTrue(containsExactlyTheIntegersFromZeroTo(destination, greatestInteger));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			DArray<int> source;
			DArray<int> dest;

			dest = source;

			Assert::IsTrue(areSizeAndCountZero(dest));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			DArray<int> source;
			DArray<int> destination;

			fillArrayWithIntegersFromZeroToWithStep(destination, INITIAL_SIZE, 1);

			destination = source;

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			DArray<int> source(INITIAL_SIZE);
			DArray<int> destination;

			fillArrayWithIntegersFromZeroToWithStep(source, INITIAL_SIZE, 1);

			destination = source;

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> source(INITIAL_SIZE);
			DArray<int> destination;

			fillArrayWithIntegersFromZeroToWithStep(source, INITIAL_SIZE, 1);
			fillArrayWithIntegersFromZeroToWithStep(destination, INITIAL_SIZE - 5, 5);

			destination = source;

			Assert::IsTrue(areEqual(source, destination));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			DArray<int> source;
			DArray<int> destination;

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			DArray<int> source;
			DArray<int> destination;

			fillArrayWithIntegersFromZeroToWithStep(destination, INITIAL_SIZE, 1);

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(areSizeAndCountZero(destination));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			DArray<int> destination;
			DArray<int> source(INITIAL_SIZE);

			const size_t greatestInteger = INITIAL_SIZE - 5;
			fillArrayWithIntegersFromZeroToWithStep(source, greatestInteger, 1);

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(destination.getCount() == greatestInteger + 1);
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE);

			Assert::IsTrue(containsExactlyTheIntegersFromZeroTo(destination, greatestInteger));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> source(INITIAL_SIZE);
			DArray<int> destination;

			const size_t greatestInteger = INITIAL_SIZE - 5;

			fillArrayWithIntegersFromZeroToWithStep(source, greatestInteger, 1);
			fillArrayWithIntegersFromZeroToWithStep(destination, INITIAL_SIZE, 5);

			destination = std::move(source);

			Assert::IsTrue(areSizeAndCountZero(source));
			Assert::IsTrue(destination.getCount() == greatestInteger + 1);
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE);

			Assert::IsTrue(containsExactlyTheIntegersFromZeroTo(destination, greatestInteger));
		}

		TEST_METHOD(testInsert)
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
		}

		TEST_METHOD(testRemoveShiftsItemsAfterTheRemovedOne)
		{
			DArray<int> dArray(INITIAL_SIZE);

			fillArrayWithIntegersFromZeroToWithStep(dArray, INITIAL_SIZE, 1);

			const int insertedCount = dArray.getCount();
			for (int i = 0; i < insertedCount; ++i)
			{
				Assert::IsTrue(dArray.getCount() == insertedCount - i);
				dArray.remove(0);
				
				if (!dArray.isEmpty())
				{
					Assert::IsTrue(dArray[0] == i + 1);
				}
			}

			Assert::IsTrue(dArray.isEmpty());
		}

		TEST_METHOD(testInsertAtLastPosition)
		{
			DArray<int> dArray(INITIAL_SIZE);

			for (int i = 0; i < INITIAL_SIZE; ++i)
			{
				dArray.insertAt(i, i);
				Assert::IsTrue(dArray.getCount() == i + 1);
				Assert::IsTrue(dArray[i] == i);
			}
		}

		TEST_METHOD(testInsertAtBetweenElements)
		{
			DArray<int> dArray(INITIAL_SIZE);

			fillArrayWithIntegersFromZeroToWithStep(dArray, INITIAL_SIZE, 1);

			const int count = dArray.getCount();

			for (int i = 0; i < INITIAL_SIZE; ++i)
			{
				for (int position = count; position >= 0; --position)
				{
					dArray.insertAt(position, i);
					Assert::IsTrue(dArray[position] == i);
				}
			}
		}

		TEST_METHOD(testIteratorIsValid)
		{
			DArray<int> darr;
			fillArrayWithIntegersFromZeroToWithStep(darr, INITIAL_SIZE, 1);

			DArray<int>::Iterator iterator = darr.getHeadIterator();

			int i = 0;
			while (iterator)
			{
				Assert::IsTrue(*iterator == i);
				++i;
				++iterator;
			}
		}

	};
}