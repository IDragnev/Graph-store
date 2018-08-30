#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../Graph store/Dynamic Array/DArray.h"

namespace DArraytest
{		
	TEST_CLASS(DArrayTest)
	{
	private:
		using UIntArray = DArray<unsigned>;

		static const size_t TEST_SIZE = 25;

		static bool areSizeAndCountZero(const UIntArray& dArray)
		{
			return dArray.getCount() == 0 && dArray.getSize() == 0;
		}

	public:
		TEST_METHOD(testDefaultConstructorConstructsEmptyArray)
		{
			UIntArray dArray;

			Assert::IsTrue(areSizeAndCountZero(dArray));
		}

		TEST_METHOD(testInitizalizerListCtor)
		{
			UIntArray dArray{ 1, 2, 3 };

			size_t i = 1;
			std::for_each(dArray.getBeginConstIterator(), dArray.getEndConstIterator(), [&](size_t current)
			{
				Assert::AreEqual(current, i++);
			});
		}

		TEST_METHOD(testConstructorFilledArrayValueInitializesAll)
		{
			UIntArray dArray(10, 10);

			for (size_t i = 0; i < 10; ++i)
			{
				Assert::AreEqual(dArray[i], 0U);
			}
		}

		TEST_METHOD(testEnsureSizeEnlargesWhenGivenSizeIsGreater)
		{
			UIntArray dArray;

			dArray.ensureSize(TEST_SIZE);

			Assert::AreEqual(dArray.getSize(), TEST_SIZE);
		}

		TEST_METHOD(testEnsureSizeDoesNotDoAnythingWhenGivenSizeIsNotGreater)
		{
			UIntArray dArray(TEST_SIZE);

			dArray.ensureSize(1);

			Assert::AreEqual(dArray.getSize(), TEST_SIZE);
		}

		TEST_METHOD(testShrinkSizeActuallyShrinksTheSize)
		{
			UIntArray dArray(TEST_SIZE);

			dArray.shrink(1);

			Assert::AreEqual(dArray.getSize(), 1U);
		}

		TEST_METHOD(testEmptyLeavesSizeZero)
		{
			UIntArray dArray{ 1, 2, 3, 4 };

			dArray.empty();

			Assert::IsTrue(dArray.isEmpty(), L"Array is not empty");
			Assert::AreEqual(dArray.getSize(), 0U, L"Size is not zero");
		}

		TEST_METHOD(testCopyCtorFromEmptySource)
		{
			UIntArray source;
			UIntArray destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(testCopyCtorFromNonEmptySource)
		{
			UIntArray source{ 1, 2, 3, 4 };
			UIntArray destination{ source };

			Assert::IsTrue(source == destination);
		}

		TEST_METHOD(testMoveCtorFromEmptySource)
		{
			UIntArray source;
			UIntArray destination{ std::move(source) };

			Assert::IsTrue(areSizeAndCountZero(source), L"Moved-in array is not empty");
			Assert::IsTrue(areSizeAndCountZero(destination), L"Moved-from array is not empty");
		}

		TEST_METHOD(testMoveCtorFromNonEmptySource)
		{
			UIntArray source{ 1, 2, 3, 4 };
			UIntArray destination{ std::move(source) };

			Assert::IsTrue(areSizeAndCountZero(source), L"Moved-from array is not empty");
			Assert::IsTrue(destination == UIntArray{ 1, 2, 3, 4 }, L"Moved-in array has invalid contents");
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			UIntArray lhs;
			UIntArray rhs;

			lhs = rhs;

			Assert::IsTrue(areSizeAndCountZero(lhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			UIntArray lhs{ 1, 2, 3, 4 };
			UIntArray rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			UIntArray lhs;
			UIntArray rhs{ 1, 2, 3, 4 };

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			UIntArray lhs{ 1, 2, 3, 4 };
			UIntArray rhs{ 5, 6, 7, 8 };
			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			UIntArray lhs;
			UIntArray rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from array is not empty");
			Assert::IsTrue(areSizeAndCountZero(lhs), L"Moved-in array is not empty");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			UIntArray lhs{ 1, 2, 3, 4 };
			UIntArray rhs;
			
			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from array is not empty");
			Assert::IsTrue(areSizeAndCountZero(lhs), L"Moved-in array is not empty");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			UIntArray lhs;
			UIntArray rhs{ 1, 2, 3, 4 };

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from array is not empty");
			Assert::IsTrue(lhs == UIntArray{ 1, 2, 3, 4 }, L"Moved-in array has invalid contents");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			UIntArray lhs{ 1, 2, 3, 4 };
			UIntArray rhs{ 5, 6, 7, 8 };

			lhs = std::move(rhs);

			Assert::IsTrue(areSizeAndCountZero(rhs), L"Moved-from array is not empty");
			Assert::IsTrue(lhs == UIntArray{ 5, 6, 7, 8 }, L"Moved-in array has invalid contents");
		}

		TEST_METHOD(testInsertLValue)
		{
			UIntArray dArray{ 1, 2, 3, 4 };

			dArray.insert(5);

			Assert::IsTrue(dArray == UIntArray{ 1, 2, 3, 4, 5 });
		}

		TEST_METHOD(testInsertRValue)
		{
			DArray<std::string> dArray{ "one", "two", "three" };
			std::string str{ "four" };
			
			dArray.insert(std::move(str));

			Assert::IsTrue(dArray == DArray<std::string>{"one", "two", "three", "four"}, L"The array has invalid contents");
			Assert::IsTrue(str == "", L"The move-inserted object is not moved");
		}

		TEST_METHOD(testRemoveAtShiftsItemsAfterTheRemovedOne)
		{
			UIntArray dArray{ 1, 2, 3, 4 };
			dArray.removeAt(0);

			Assert::AreEqual(dArray.getCount(), 3U, L"Count is not updated");
			Assert::IsTrue(dArray == UIntArray{ 2, 3, 4 });
		}

		TEST_METHOD(testInsertAt)
		{
			UIntArray dArray{ 2, 3, 4, 5 };

			dArray.insertAt(0, 1);
			
			Assert::IsTrue(dArray == UIntArray{ 1, 2, 3, 4, 5 });
		}
	};
}