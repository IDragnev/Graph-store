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
		TEST_METHOD(DefaultConstructedIntegerIsInfinite)
		{
			constexpr SpecialInteger integer;

			Assert::IsTrue(integer.isInfinite());
		}

		TEST_METHOD(ConvertedBuiltInIntegerIsFinite)
		{
			SpecialInteger integer{ 1 };

			Assert::IsFalse(integer.isInfinite());
		}

		TEST_METHOD(AssignmentFromFiniteToFinite)
		{
			SpecialInteger integer{ 0 };
			
			integer = 1;

			Assert::IsFalse(integer.isInfinite());
		}

		TEST_METHOD(AssignmentToInfiniteToBuiltInFinite)
		{
			SpecialInteger integer;

			integer = 1;

			Assert::IsFalse(integer.isInfinite());
		}

		TEST_METHOD(AssignmentToFiniteFromInfinite)
		{
			SpecialInteger integer{ 0 };

			integer = infinity;

			Assert::IsTrue(integer.isInfinite());
		}

		TEST_METHOD(AssignmentToInfiniteFromInfinite)
		{
			SpecialInteger integer;

			integer = infinity;

			Assert::IsTrue(integer.isInfinite());
		}

		TEST_METHOD(CopyCtorFromInfinite)
		{
			auto integer = infinity;

			Assert::IsTrue(integer.isInfinite());
		}

		TEST_METHOD(CopyCtorFromFinite)
		{
			SpecialInteger source{ 1 };
			SpecialInteger destination{ source };

			Assert::IsFalse(destination.isInfinite());
		}

		TEST_METHOD(AddingBuiltInIntegerToInfinityDoesNothing)
		{
			auto integer = infinity;

			integer += 1;

			Assert::IsTrue(integer.isInfinite());
		}

		TEST_METHOD(AddingInfinityToInfinityDoesNothing)
		{
			auto integer = infinity;

			integer += infinity;

			Assert::IsTrue(integer.isInfinite());
		}

		TEST_METHOD(AddingInfinityToFiniteMakesItInfinite)
		{
			SpecialInteger lhs{ 1 };

			lhs += infinity;

			Assert::IsTrue(lhs.isInfinite());
		}

		TEST_METHOD(AddingBuiltInFiniteToFinite)
		{
			SpecialInteger lhs{ 1 };
			SpecialInteger expected{ 2 };

			lhs += 1;

			Assert::IsFalse(lhs.isInfinite(), L"Lhs is equal to infinity");
			Assert::IsTrue(lhs == expected, L"Lhs has incorrect value");
		}
		TEST_METHOD(ComparingInfinityWithItself)
		{
			Assert::IsFalse(infinity < infinity);
			Assert::IsFalse(infinity > infinity);
			Assert::IsFalse(infinity == infinity);

			Assert::IsTrue(infinity >= infinity);
			Assert::IsTrue(infinity <= infinity);
			Assert::IsTrue(infinity != infinity);
		}

		TEST_METHOD(InfinityIsGreaterThanTheMaxOfUnderlyingType)
		{
			using T = SpecialInteger::UnderlyingType;

			SpecialInteger fromMaxOfUnderlying = std::numeric_limits<T>::max();

			Assert::IsTrue(fromMaxOfUnderlying < infinity);
		}

		private:
			static inline constexpr SpecialInteger infinity{};
	};
}