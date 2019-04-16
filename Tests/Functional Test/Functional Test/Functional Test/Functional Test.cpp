#include "CppUnitTest.h"
#include "..\..\..\..\Graph store\Functional\Functional.h"
#include <algorithm>
#include <string>
#include <vector>

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

			Assert::AreEqual(1, Identity{}(1));
			Assert::AreNotEqual(2, Identity{}(1));
			auto x = 0;
			Assert::AreSame(x, Identity{}(x));
			Assert::AreNotSame(0, Identity{}(x));
		}

		TEST_METHOD(superposition)
		{
			auto h = superpose(sum, compose(plus(10), plus(123)), Identity{});
			Assert::AreEqual(h(1), 135);
			Assert::AreEqual(superpose(sum, plus(10), plus(35))(1), 47);
			Assert::AreEqual(superpose(sum, multiply, sum)(10, 20), 230);
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

		TEST_METHOD(composition)
		{
			auto g = compose(plus(8), plus(10), plus(35), Identity{}, plus(100));
			Assert::AreEqual(g(2), 155);
			Assert::AreEqual(compose(plus("789"s), plus("456"s), toString)(123), "123456789"s);
		}

		TEST_METHOD(higherOrderEqualTo)
		{
			Assert::IsTrue(equalTo(1)(1));
			Assert::IsFalse(equalTo(2u)(3u));
			Assert::IsTrue(equalTo("123"s)("123"s));
			Assert::IsFalse(equalTo("lhs"s)("rhs"s));
		}

		TEST_METHOD(matchingByKey)
		{
			struct Item
			{
				std::string key = "target";
			} first, second{ "s" };
			auto extractKey = [](const auto& x) { return x.key; };
			auto matchesKey = matches(first.key, extractKey);

			Assert::IsTrue(matchesKey(first));
			Assert::IsFalse(matchesKey(second));
		}
		
		TEST_METHOD(inversingAPredicate)
		{
			auto predicate = [](int x) { return x >= 0; };
			auto nums = { 1, 2, -1, 2 };

			auto it = std::find_if(nums.begin(), nums.end(), inverse(predicate));

			Assert::IsNotNull(it);
			Assert::AreEqual(-1, *it);
		}

		TEST_METHOD(plusTakesLhs)
		{
			using Strings = std::vector<std::string>;
			auto strings = Strings{ "a", "b", "c" };
			auto expected = Strings{ "a!", "b!", "c!" };

			std::transform(std::begin(strings), std::end(strings), std::begin(strings), plus("!"));

			Assert::IsTrue(strings == expected);
		}
	};
}