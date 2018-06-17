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

	bool haveSameContents(const DArray<int>& lhs, const DArray<int>& rhs)
	{
		const int lhsCount = lhs.getCount();

		if (lhsCount != rhs.getCount())
		{
			return false;
		}

		for (int i = 0; i < lhsCount; ++i)
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

	bool containsExactlyTheIntegersFromZeroTo(const DArray<int>& dArray, const int integer)
	{
		if (dArray.getCount() != integer + 1)
		{
			return false;
		}

		for (int i = 0; i <= integer; ++i)
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

			Assert::IsTrue(dArray.isEmpty(), L"Object is not empty after calling empty");
			Assert::IsTrue(dArray.getSize() == 0, L"Size is not zero after calling empty");
		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			DArray<int> source;
			DArray<int> destination(source);

			Assert::AreEqual(source.getSize(), destination.getSize(), L"Copy ctor does not manage size properly");
			Assert::IsTrue(haveSameContents(source, destination), L"Destination has different contents from source");
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			DArray<int> source(INITIAL_SIZE);
			fillArrayWithIntegersFromZeroToWithStep(source, INITIAL_SIZE + 10, 1);

			DArray<int> destination(source);

			Assert::AreEqual(source.getSize(), destination.getSize(), L"Copy ctor does not manage size properly");
			Assert::IsTrue(haveSameContents(source, destination), L"Destination has different contents from source");
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			DArray<int> source;
			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source), L"Moved-in object is not empty");
			Assert::IsTrue(areSizeAndCountZero(destination), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			DArray<int> source(INITIAL_SIZE);

			const size_t greatestInteger = INITIAL_SIZE - 5;
			fillArrayWithIntegersFromZeroToWithStep(source, greatestInteger, 1);

			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source), L"Moved-from object is not empty");

			Assert::IsTrue(destination.getSize() == INITIAL_SIZE, L"Size is not set properly in move ctor");
			Assert::IsTrue(containsExactlyTheIntegersFromZeroTo(destination, greatestInteger), L"Destination has different contents from the moved-from object");
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs;

			lhs = rhs;

			Assert::IsTrue(areSizeAndCountZero(lhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs;

			fillArrayWithIntegersFromZeroToWithStep(lhs, INITIAL_SIZE, 1);

			lhs = rhs;

			Assert::AreEqual(lhs.getSize(), rhs.getSize(), L"Rhs and Lhs have different sizes");
			Assert::IsTrue(haveSameContents(lhs, rhs), L"Rhs has different contents from Lhs");
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			fillArrayWithIntegersFromZeroToWithStep(rhs, INITIAL_SIZE, 1);

			lhs = rhs;

			Assert::AreEqual(lhs.getSize(), rhs.getSize(), L"Rhs and Lhs have different sizes");
			Assert::IsTrue(haveSameContents(lhs, rhs), L"Rhs has different contents from Lhs");
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			fillArrayWithIntegersFromZeroToWithStep(rhs, INITIAL_SIZE, 1);
			fillArrayWithIntegersFromZeroToWithStep(lhs, INITIAL_SIZE - 5, 5);

			lhs = rhs;

			Assert::AreEqual(lhs.getSize(), rhs.getSize(), L"Rhs and Lhs have different sizes");
			Assert::IsTrue(haveSameContents(lhs, rhs), L"Rhs has different contents from Lhs");
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(areSizeAndCountZero(lhs), L"Moved-in object is not empty");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs;

			fillArrayWithIntegersFromZeroToWithStep(lhs, INITIAL_SIZE, 1);

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(areSizeAndCountZero(lhs), L"Moved-in object is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			const size_t greatestInteger = INITIAL_SIZE - 5;
			fillArrayWithIntegersFromZeroToWithStep(rhs, greatestInteger, 1);

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.getSize() == INITIAL_SIZE, L"Size is not set properly in move assignment");
			Assert::IsTrue(containsExactlyTheIntegersFromZeroTo(lhs, greatestInteger), L"Lhs has different contents from the moved-in contents");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			const size_t greatestInteger = INITIAL_SIZE - 5;

			fillArrayWithIntegersFromZeroToWithStep(rhs, greatestInteger, 1);
			fillArrayWithIntegersFromZeroToWithStep(lhs, INITIAL_SIZE, 5);

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.getSize() == INITIAL_SIZE, L"Size is not set properly in move assignment");
			Assert::IsTrue(containsExactlyTheIntegersFromZeroTo(lhs, greatestInteger), L"Lhs has different contents from the moved-in contents");
		}

		TEST_METHOD(testInsertUpdatesCountAndInsertsBack)
		{
			DArray<int> dArray(INITIAL_SIZE);

			for (int i = 1; i < INITIAL_SIZE; ++i)
			{
				dArray.insert(i);
				Assert::IsTrue(dArray.getCount() == i, L"Count is not updated when inserting");
				Assert::IsTrue(dArray[i - 1] == i, L"Item is not inserted at back");
			}
		}

		TEST_METHOD(testRemoveShiftsItemsAfterTheRemovedOne)
		{
			DArray<int> dArray(INITIAL_SIZE);

			fillArrayWithIntegersFromZeroToWithStep(dArray, INITIAL_SIZE, 1);

			const int insertedCount = dArray.getCount();
			for (int i = 0; i < insertedCount; ++i)
			{
				dArray.remove(0);

				Assert::IsTrue(dArray.getCount() == insertedCount - (i + 1));
				
				if (!dArray.isEmpty())
				{
					Assert::IsTrue(dArray[0] == i + 1);
				}
			}
		}

		TEST_METHOD(testInsertAtLastPosition)
		{
			DArray<int> dArray(INITIAL_SIZE);

			for (int i = 0; i < INITIAL_SIZE; ++i)
			{
				dArray.insertAt(i, i);
				Assert::IsTrue(dArray.getCount() == i + 1, L"insertAt does not update count");
				Assert::IsTrue(dArray[i] == i, L"The item at the inserted position is different");
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
					Assert::IsTrue(dArray[position] == i, L"The item at the inserted position is different");
				}
			}
		}
	};
}