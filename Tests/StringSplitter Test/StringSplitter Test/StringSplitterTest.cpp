#include "CppUnitTest.h"
#include "..\..\..\Graph store\Application\StringSplitter\StringSplitter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		using Splitter = StringSplitter<DArray>;
		using Container = DArray<std::string>;

	public:	
		TEST_METHOD(testDefaultSplitterHasOnlyWhiteSpaceDelimiter)
		{
			Splitter s;

			Container result = s.split("one two 'three'");

			Assert::IsTrue(result == Container{ "one", "two", "'three'"});
		}

		TEST_METHOD(testSplittingTheEmptyStringReturnsEmptyContainer)
		{
			Splitter s{ ' ', '\'' };

			Container result = s.split("");

			Assert::IsTrue(result.isEmpty());
		}

		TEST_METHOD(testWhiteSpacesAreIgnored)
		{
			Splitter s{ '\"', '\'' };

			Container result = s.split("     'one'     ''  \"-two-\"    \"three\"      '*four*'         \"five\"");

			Assert::IsTrue(result == Container{ "one", "", "-two-", "three", "*four*", "five" });
		}

		TEST_METHOD(testWhiteSpaceIsAlwaysConsideredAsDelimiter)
		{
			Splitter s{ '\'' };

			Container result = s.split("     'one'   two   'three and a half'      *four*  ");

			Assert::IsTrue(result == Container{ "one", "two", "three and a half", "*four*" });
		}

		TEST_METHOD(testWhiteSpaceIsMatchedByNullTerminatingCharacter)
		{
			Splitter s{ '\'' };

			Container result = s.split(" one two");

			Assert::IsTrue(result == Container{ "one", "two" });
		}

		TEST_METHOD(testNonWhiteSpaceDelimiterIsNotMatchedByNullTerminatingCharacter)
		{
			Splitter s{ '\'' };

			Container result = s.split(" one 'two  ");

			Assert::IsTrue(result == Container{ "one" });
		}
	};
}