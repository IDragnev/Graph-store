#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../Graph store/Dynamic Array/DArray.h"

namespace DArraytest
{		
	void fillArrayWithIntegersFromRange(DArray<int>& arr, int smallest, int biggest)
	{
		for (int i = smallest; i <= biggest; ++i)
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

	bool containsExactlyTheIntegersFromTo(const DArray<int>& dArray, int smallest, int biggest)
	{
		int count = dArray.getCount();

		if (count != biggest - smallest + 1)
		{
			return false;
		}

		int current = smallest;
		for (int i = 0; i < count; ++i)
		{
			if (dArray[i] != current++)
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
		TEST_METHOD(testDefaultConstructorConstructsEmptyArray)
		{
			DArray<int> dArray;

			Assert::IsTrue(areSizeAndCountZero(dArray));
		}

		TEST_METHOD(testEnsureSizeEnlargesWhenGivenSizeIsGreater)
		{
			DArray<int> dArray;

			dArray.ensureSize(INITIAL_SIZE);
			Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
		}

		TEST_METHOD(testEnsureSizeDoesNotDoAnythingWhenGivenSizeIsNotGreater)
		{
			DArray<int> dArray(INITIAL_SIZE);

			dArray.ensureSize(1);
			Assert::IsTrue(dArray.getSize() == INITIAL_SIZE);
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
			fillArrayWithIntegersFromRange(dArray, 1, INITIAL_SIZE);

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
			fillArrayWithIntegersFromRange(source, 1, INITIAL_SIZE);

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

			fillArrayWithIntegersFromRange(source, 1, INITIAL_SIZE);

			DArray<int> destination(std::move(source));

			Assert::IsTrue(areSizeAndCountZero(source), L"Moved-from object is not empty");
			Assert::IsTrue(destination.getSize() == INITIAL_SIZE, L"Size is not set properly in move ctor");
			Assert::IsTrue(containsExactlyTheIntegersFromTo(destination, 1, INITIAL_SIZE), L"Destination has different contents from the moved-from object");
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

			fillArrayWithIntegersFromRange(lhs, 1, INITIAL_SIZE);

			lhs = rhs;

			Assert::AreEqual(lhs.getSize(), rhs.getSize(), L"Rhs and Lhs have different sizes");
			Assert::IsTrue(haveSameContents(lhs, rhs), L"Rhs has different contents from Lhs");
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			fillArrayWithIntegersFromRange(rhs, 1, INITIAL_SIZE);

			lhs = rhs;

			Assert::AreEqual(lhs.getSize(), rhs.getSize(), L"Rhs and Lhs have different sizes");
			Assert::IsTrue(haveSameContents(lhs, rhs), L"Rhs has different contents from Lhs");
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			fillArrayWithIntegersFromRange(rhs, 1, INITIAL_SIZE);
			fillArrayWithIntegersFromRange(lhs, 2, INITIAL_SIZE);

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

			fillArrayWithIntegersFromRange(lhs, 1, INITIAL_SIZE);

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(areSizeAndCountZero(lhs), L"Moved-in object is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			fillArrayWithIntegersFromRange(rhs, 1, INITIAL_SIZE);

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.getSize() == INITIAL_SIZE, L"Size is not set properly in move assignment");
			Assert::IsTrue(containsExactlyTheIntegersFromTo(lhs, 1, INITIAL_SIZE), L"Lhs has different contents from the moved-in contents");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			DArray<int> lhs;
			DArray<int> rhs(INITIAL_SIZE);

			fillArrayWithIntegersFromRange(rhs, 1, INITIAL_SIZE);
			fillArrayWithIntegersFromRange(lhs, 2, INITIAL_SIZE);

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from object is not empty");
			Assert::IsTrue(lhs.getSize() == INITIAL_SIZE, L"Size is not set properly in move assignment");
			Assert::IsTrue(containsExactlyTheIntegersFromTo(lhs, 1, INITIAL_SIZE), L"Lhs has different contents from the moved-in contents");
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

			fillArrayWithIntegersFromRange(dArray, 1, INITIAL_SIZE);

			dArray.remove(0);

			Assert::IsTrue(dArray.getCount() == INITIAL_SIZE - 1, L"removeAt does not update count");
			Assert::IsTrue(containsExactlyTheIntegersFromTo(dArray, 2, INITIAL_SIZE));
		}

		TEST_METHOD(testInsertAt)
		{
			DArray<int> dArray(INITIAL_SIZE);

			fillArrayWithIntegersFromRange(dArray, 1, 10);

			dArray.insertAt(5, 100);
			
			Assert::IsTrue(dArray[5] == 100, L"The item at the inserted position is different");
			Assert::IsTrue(dArray.getCount() == 11, L"insertAt does not update count");

			for (int i = 6; i < 11; ++i)
			{
				Assert::IsTrue(dArray[i] == i, L"Items after the inserted position are not shifted properly");
			}
		}
	};
}