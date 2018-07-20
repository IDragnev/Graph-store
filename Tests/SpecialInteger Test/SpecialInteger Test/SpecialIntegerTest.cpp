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

			integer = SpecialInteger::getInfinity();

			Assert::IsTrue(integer.isEqualToInfinity());
		}

		TEST_METHOD(testAssignmentToInfinityFromInfinityLeavesItToInfinity)
		{
			SpecialInteger infinity;

			infinity = SpecialInteger::getInfinity();

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

	};
}