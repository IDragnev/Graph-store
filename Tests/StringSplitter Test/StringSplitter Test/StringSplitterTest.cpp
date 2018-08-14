#include "CppUnitTest.h"
#include "..\..\..\Graph store\Application\StringSplitter\StringSplitter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		typedef std::vector<std::string> Container;

	public:	
		TEST_METHOD(testDefaultSplitterHasOnlyWhiteSpaceDelimiter)
		{
			StringSplitter<> splitter;

			Container result = splitter.split("one two 'three'");

			Assert::IsTrue(result == Container{ "one", "two", "'three'"});
		}

		TEST_METHOD(testSplittingTheEmptyStringReturnsEmptyContainer)
		{
			StringSplitter<> splitter{ ' ', '\'' };

			Container result = splitter.split("");

			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(testWhiteSpacesAreIgnored)
		{
			StringSplitter<> splitter{ '\"', '\'' };

			Container result = splitter.split("     'one'     ''  \"-two-\"    \"three\"      '*four*'         \"five\"");

			Assert::IsTrue(result == Container{ "one", "", "-two-", "three", "*four*", "five" });
		}

		TEST_METHOD(testWhiteSpaceIsAlwaysConsideredAsDelimiter)
		{
			StringSplitter<> splitter{ '\'' };

			Container result = splitter.split("     'one'   two   'three and a half'      *four*  ");

			Assert::IsTrue(result == Container{ "one", "two", "three and a half", "*four*" });
		}

		TEST_METHOD(testWhiteSpaceIsMatchedByNullTerminatingCharacter)
		{
			StringSplitter<> splitter{ '\'' };

			Container result = splitter.split(" one two");

			Assert::IsTrue(result == Container{ "one", "two" });
		}

		TEST_METHOD(testNonWhiteSpaceDelimiterIsNotMatchedByNullTerminatingCharacter)
		{
			StringSplitter<> splitter{ '\'' };

			Container result = splitter.split(" one 'two  ");

			Assert::IsTrue(result == Container{ "one" });
		}
	};
}