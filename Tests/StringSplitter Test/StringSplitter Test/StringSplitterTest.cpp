#include "CppUnitTest.h"
#include "StringSplitter\StringSplitter.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		using Splitter = IDragnev::StringSplitter<>;
		using Container = decltype(Splitter{}("s"));

	public:	
		TEST_METHOD(defaultSplitterDelimitsByWhiteSpace)
		{
			Assert::IsTrue(Splitter{}("one two 'three'") == Container{ "one", "two", "'three'"});
		}

		TEST_METHOD(splittingTheEmptyStringReturnsEmptyResult)
		{
			Splitter splitter{ ' ', '*' };

			auto result = splitter("");

			Assert::IsTrue(result.isEmpty());
		}

		TEST_METHOD(whiteSpacesAreIgnored)
		{
			Splitter splitter{ '\"', '\'' };

			auto result = splitter("     'one'     ''  \"-two-\"    \"three\"      '*four*'         \"five\"");

			Assert::IsTrue(result == Container{ "one", "", "-two-", "three", "*four*", "five" });
		}

		TEST_METHOD(whiteSpaceIsAlwaysConsideredAsDelimiter)
		{
			Splitter splitter{ '\'' };

			auto result = splitter("     'one'   two   'three and a half'      *four*  ");

			Assert::IsTrue(result == Container{ "one", "two", "three and a half", "*four*" });
		}

		TEST_METHOD(whiteSpaceIsMatchedByNullTerminatingCharacter)
		{
			Splitter splitter{ '\'' };

			Assert::IsTrue(splitter(" 'one' two") == Container{ "one", "two" });
		}

		TEST_METHOD(unmatchedDelimiterThrows)
		{
			Splitter splitter{ '\'' };

			try
			{
				splitter(" one 'two  ");

				Assert::Fail(L"splitter did not throw");
			}
			catch (std::runtime_error&)
			{
			}
		}

		TEST_METHOD(movedFromSplitterDelimitsAsDefaultConstructed)
		{
			Splitter source{ '@', '*' };
			auto destination = std::move(source);
			auto input = "@one@ *two* three"s;

			Assert::IsTrue(source(input) == Container{ "@one@", "*two*", "three" }, L"moved-from splitter has invalid result");
			Assert::IsTrue(destination(input) == Container{ "one", "two", "three" }, L"moved-in splitter has invalid result");
		}

		TEST_METHOD(copyConstructor)
		{
			Splitter source{ '@', '*' };
			auto destination = source;
			auto input = "@one@ *two* three"s;
			auto expected = Container{ "one", "two", "three" };

			Assert::IsTrue(source(input) == expected);
			Assert::IsTrue(destination(input) == expected);
		}

		TEST_METHOD(moveAssignedFromSplitterDelimitsAsDefaultConstructed)
		{
			Splitter rhs{ '@', '*' };
			Splitter lhs;
			auto input = "@one@ *two* three"s;

			lhs = std::move(rhs);

			Assert::IsTrue(lhs(input) == Container{ "one", "two", "three" }, L"moved-in splitter has invalid result");
			Assert::IsTrue(rhs(input) == Container{ "@one@", "*two*", "three" }, L"moved-from splitter has invalid result");
		}

		TEST_METHOD(copyAssignment)
		{
			Splitter rhs{ '@', '*' };
			Splitter lhs;
			auto string = "@one@ *two* three"s;
			auto expected = Container{ "one", "two", "three" };

			lhs = rhs;

			Assert::IsTrue(lhs(string) == expected);
			Assert::IsTrue(rhs(string) == expected);
		}

		TEST_METHOD(multipleCalls)
		{
			using Containers = std::vector<Container>;

			Splitter splitter{ '*', '@' };
			auto input = { "a *b*", "@c@ d", "@e@ f" };
			auto expected = Containers{ {"a", "b"}, {"c", "d"}, {"e", "f"} };
			auto actual = Containers{};

			std::transform(std::begin(input), std::end(input),
				           std::back_inserter(actual), splitter);

			Assert::IsTrue(expected == actual);
		}
	};
}