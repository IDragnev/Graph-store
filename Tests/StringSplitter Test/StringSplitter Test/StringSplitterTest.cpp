#include "CppUnitTest.h"
#include "..\..\..\Graph store\StringSplitter\StringSplitter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::Containers;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		using Splitter = IDragnev::StringSplitter<>;
		using Container = decltype(Splitter{}("s"));

	public:	
		TEST_METHOD(defaultSplitterHasOnlyWhiteSpaceDelimiter)
		{
			Assert::IsTrue(Splitter{}("one two 'three'") == Container{ "one", "two", "'three'"});
		}

		TEST_METHOD(splittingTheEmptyStringReturnsEmptyContainer)
		{
			Splitter splitter{ ' ', '\'' };

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

			Assert::IsTrue(splitter(" one two") == Container{ "one", "two" });
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
	};
}