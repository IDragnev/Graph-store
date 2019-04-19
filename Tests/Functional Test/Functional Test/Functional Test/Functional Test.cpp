#include "CppUnitTest.h"
#include "Functional\Functional.h"
#include <algorithm>
#include <string>
#include <vector>
#include <array>

using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::Functional;

namespace FunctionalTest
{		
	auto sum = [](auto x, auto y) { return x + y; };
	auto multiply = [](auto x, auto y) { return x * y; };
	auto toString = [](auto num) { return std::to_string(num); };

	TEST_CLASS(FunctionalTest)
	{
	public:
		TEST_METHOD(trivialFunctors)
		{
			Assert::IsTrue(LessThan{}(1, 2));
			Assert::IsFalse(LessThan{}(1, 1));
			Assert::IsFalse(LessThan{}(2, 1));

			Assert::IsTrue(EqualTo{}(1, 1));
			Assert::IsFalse(EqualTo{}(1, 2));

			constexpr auto x = LessThan{}(1, 2) && EqualTo{}(1, 2);
		}

		TEST_METHOD(identity)
		{
			Assert::AreEqual(1, Identity{}(1));
			Assert::AreNotEqual(2, Identity{}(1));

			auto x = 0;
			Assert::AreSame(x, Identity{}(x));
			Assert::AreNotSame(0, Identity{}(x));

			struct A{ };
			A&& a = Identity{}(A{});

			constexpr auto y = Identity{}(1);
		}

		TEST_METHOD(plusTakesLeftOperand)
		{
			using Strings = std::vector<std::string>;
			auto strings = Strings{ "a", "b", "c" };
			auto expected = Strings{ "a!", "b!", "c!" };

			std::transform(std::begin(strings), std::end(strings), std::begin(strings), plus("!"));

			Assert::IsTrue(strings == expected);
		}
		TEST_METHOD(superposition)
		{
			auto f = [](auto x, auto y) { return x >= y; };
			auto g = superpose(f, multiply, sum);
			auto h = superpose(f, sum, multiply);

			Assert::IsTrue(g(2, 3));
			Assert::IsFalse(h(2, 3));
		}

		TEST_METHOD(composition)
		{
			auto f = compose(plus("789"s), plus("456"s), toString, Identity{});
			
			Assert::AreEqual(f(123), "123456789"s);
		}

		TEST_METHOD(superpositionAndCompositionHandleReferences)
		{
			auto source = "123"s;
			auto copy = source;
			auto f = superpose(Identity{}, Identity{});
			auto g = compose(f, f);

			Assert::AreSame(source, f(source));
			Assert::AreNotSame(source, f(copy));
			Assert::AreSame(source, g(source));
			Assert::AreNotSame(source, g(copy));
		}

		TEST_METHOD(superpositionAndCompositionCanComputeAtCompileTime)
		{
			constexpr auto plusOne = [](auto x) constexpr { return x + 1; };
			constexpr auto minusOne = [](auto x) constexpr { return x - 1; };
			constexpr auto areEqual = [](auto x, auto y) constexpr { return x == y; };

			using ArrayOfSizeTwo = std::array<int, superpose(areEqual, plusOne, minusOne)(4) + 2>;
			using ArrayOfSizeOne = std::array<int, compose(minusOne, plusOne)(1)>;
		}

		TEST_METHOD(higherOrderEqualTo)
		{
			Assert::IsTrue(equalTo(1)(1));
			Assert::IsFalse(equalTo(2u)(3u));

			Assert::IsTrue(equalTo("123"s)("123"s));
			Assert::IsFalse(equalTo("lhs"s)("rhs"s));

			struct X 
			{
				int x = 1;
				operator int() const { return x; }
			};

			Assert::IsTrue(equalTo(1)(X{}));
		}

		TEST_METHOD(matchingByKey)
		{
			struct Item
			{
				std::string key = "target";
			} first, second{ "s" };
			auto extractKey = [](const auto& x) { return x.key; };
			auto matchesTarget = matches(first.key, extractKey);

			Assert::IsTrue(matchesTarget(first));
			Assert::IsFalse(matchesTarget(second));
		}
		
		TEST_METHOD(inversingAPredicate)
		{
			auto predicate = [](int x) { return x >= 0; };
			auto nums = { 1, 2, -1, 2 };

			auto it = std::find_if(std::begin(nums), std::end(nums), inverse(predicate));

			Assert::IsNotNull(it);
			Assert::AreEqual(-1, *it);
		}
	};
}