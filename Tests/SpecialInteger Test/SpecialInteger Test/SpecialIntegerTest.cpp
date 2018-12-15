#include "CppUnitTest.h"
#include "../../../Graph store/Special Integer/SpecialInteger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SpecialIntegerTest
{		

	TEST_CLASS(SpecialIntegerTest)
	{
	private:
		using SpecialInteger = IDragnev::SpecialInteger<int>;

	public:
		TEST_METHOD(DefaultConstructedIntegerIsEqualToInfinity)
		{
			SpecialInteger integer;

			Assert::IsTrue(integer.isEqualToInfinity());
		}

		TEST_METHOD(ConversionCtorConstructsAnIntegerNotEqualToInfinity)
		{
			SpecialInteger integer{ 1 };

			Assert::IsFalse(integer.isEqualToInfinity());
		}

		TEST_METHOD(AssignmentToNonInfinityFromPrimitiveIntegerLeavesItToNonInfinity)
		{
			SpecialInteger integer{ 0 };
			
			integer = 1;

			Assert::IsFalse(integer.isEqualToInfinity());
		}

		TEST_METHOD(AssignmentToInfinityFromPrimitiveIntegerConvertsItToNonInfinity)
		{
			SpecialInteger infinity;

			infinity = 1;

			Assert::IsFalse(infinity.isEqualToInfinity());
		}

		TEST_METHOD(AssignmentToNonInfinityFromInfinityConvertsItToInfinity)
		{
			SpecialInteger integer{ 0 };

			integer = SpecialInteger::Infinity();

			Assert::IsTrue(integer.isEqualToInfinity());
		}

		TEST_METHOD(AssignmentToInfinityFromInfinityLeavesItToInfinity)
		{
			SpecialInteger infinity;

			infinity = SpecialInteger::Infinity();

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

		TEST_METHOD(CopyCtorFromInfinityConstructsInfinity)
		{
			SpecialInteger integer = SpecialInteger::Infinity();

			Assert::IsTrue(integer.isEqualToInfinity());
		}

		TEST_METHOD(CopyCtorFromNonInfinityConstructsNonInfinity)
		{
			SpecialInteger source{ 1 };
			SpecialInteger destination{ source };

			Assert::IsFalse(destination.isEqualToInfinity());
		}

		TEST_METHOD(AddingIntegerToInfinityDoesNothing)
		{
			SpecialInteger infinity;

			infinity += 1;

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

		TEST_METHOD(AddingInfinityToInfinityDoesNothing)
		{
			SpecialInteger infinity;

			infinity += SpecialInteger::Infinity();

			Assert::IsTrue(infinity.isEqualToInfinity());
		}

		TEST_METHOD(AddingInfinityToNonInfinityConvertsLhsToInfinity)
		{
			SpecialInteger lhs{ 1 };

			lhs += SpecialInteger::Infinity();

			Assert::IsTrue(lhs.isEqualToInfinity());
		}

		TEST_METHOD(AddingNonInfinityToNonInfinityDoesNotMakeItInfinity)
		{
			SpecialInteger lhs{ 1 };

			lhs += 1;

			Assert::IsFalse(lhs.isEqualToInfinity(), L"Lhs is equal to infinity");
			Assert::IsTrue(lhs == SpecialInteger{ 2 }, L"Lhs has incorrect value");
		}

		TEST_METHOD(InfinityIsNotSmallerThanInfinity)
		{
			SpecialInteger infinity1;
			SpecialInteger infinity2;

			Assert::IsFalse(infinity1 < infinity2);
		}

		TEST_METHOD(InfinityIsNotEqualToInfinity)
		{
			SpecialInteger infinity1;
			SpecialInteger infinity2;

			Assert::IsFalse(infinity1 == infinity2);
		}

		TEST_METHOD(InfinityIsNotEqualToMaxBuiltInInteger)
		{
			SpecialInteger maxBuiltIn = std::numeric_limits<int>::max();

			Assert::IsTrue(maxBuiltIn != SpecialInteger::Infinity());
		}

		TEST_METHOD(InfinityIsGreaterThanAllBuiltIntegers)
		{
			SpecialInteger maxBuiltIn = std::numeric_limits<int>::max();
			
			Assert::IsTrue(maxBuiltIn < SpecialInteger::Infinity());
		}
	};
}