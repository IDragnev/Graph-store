#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/Special Integer/SpecialInteger.h"


namespace SpecialIntegerTest
{		
	TEST_CLASS(SpecialIntegerTest)
	{
	private:
		typedef SpecialInteger<int> SpecialInteger;

	public:
		TEST_METHOD(testDefaultConstructedIntegerIsEqualToInfinity)
		{
			SpecialInteger integer;

			Assert::IsTrue(integer.isEqualToInfinity());
		}

		TEST_METHOD(testConversionCtorConstructsAnIntegerNotEqualToInfinity)
		{
			SpecialInteger integer(1);

			Assert::IsFalse(integer.isEqualToInfinity());
		}

		TEST_METHOD(testAssignmentToNonInfinityFromPrimitiveIntegerLeavesItToNonInfinity)
		{
			SpecialInteger integer(0);
			
			integer = 1;

			Assert::IsFalse(integer.isEqualToInfinity());
		}

		TEST_METHOD(testAssignmentToInfinityFromPrimitiveIntegerConvertsItToNonInfinity)
		{
			SpecialInteger infinity;

			infinity = 1;

			Assert::IsFalse(infinity.isEqualToInfinity());
		}

		TEST_METHOD(testAssignmentToNonInfinityFromInfinityConvertsItToInfinity)
		{
			SpecialInteger integer(0);

			integer = SpecialInteger::Infinity();

			Assert::IsTrue(integer.isEqualToInfinity());
		}

		TEST_METHOD(testAssignmentToInfinityFromInfinityLeavesItToInfinity)
		{
			SpecialInteger infinity;

			infinity = SpecialInteger::Infinity();

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

		TEST_METHOD(testCopyCtorFromInfinityConstructsInfinity)
		{
			SpecialInteger copyOfInfinity = SpecialInteger::Infinity();

			Assert::IsTrue(copyOfInfinity.isEqualToInfinity());
		}

		TEST_METHOD(testCopyCtorFromNonInfinityConstructsNonInfinity)
		{
			SpecialInteger source(1);
			SpecialInteger destination(source);

			Assert::IsFalse(destination.isEqualToInfinity());
		}

		TEST_METHOD(testAddingIntegerToInfinityDoesNothing)
		{
			SpecialInteger infinity;

			infinity += 1;

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

		TEST_METHOD(testAddingInfinityToInfinityDoesNothing)
		{
			SpecialInteger infinity;

			infinity += SpecialInteger::Infinity();

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

		TEST_METHOD(testAddingInfinityToNonInfinityMakesLHSInfinity)
		{
			SpecialInteger lhs(1);

			lhs += SpecialInteger::Infinity();

			Assert::IsTrue(lhs.isEqualToInfinity());
		}

		TEST_METHOD(testAddingNonInfinityToNonInfinityDoesNotMakeItInfinity)
		{
			SpecialInteger lhs(1);

			lhs += 1;

			Assert::IsFalse(lhs.isEqualToInfinity(), L"Lhs is equal to infinity after adding non-infinity to it");
			Assert::IsTrue(lhs == SpecialInteger(2), L"Lhs has wrong value after calling operator+=");
		}

		TEST_METHOD(testInfinityIsNotSmallerThanInfinity)
		{
			SpecialInteger infinity1;
			SpecialInteger infinity2;

			Assert::IsFalse(infinity1 < infinity2);
		}

		TEST_METHOD(testInfinityIsNotEqualToInfinity)
		{
			SpecialInteger infinity1;
			SpecialInteger infinity2;

			Assert::IsFalse(infinity1 == infinity2);
		}

		TEST_METHOD(testInfinityIsNotEqualToMaxPrimitiveInteger)
		{
			SpecialInteger maxPrimitive = std::numeric_limits<int>::max();

			Assert::IsTrue(maxPrimitive != SpecialInteger::Infinity());
		}

		TEST_METHOD(testInfinityIsGreaterThanAllPrimitiveIntegers)
		{
			SpecialInteger maxPrimitive = std::numeric_limits<int>::max();
			
			Assert::IsTrue(maxPrimitive < SpecialInteger::Infinity());
		}
	};
}