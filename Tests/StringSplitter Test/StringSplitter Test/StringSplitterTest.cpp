#include "CppUnitTest.h"
#include "..\..\..\Graph store\Application\StringSplitter\StringSplitter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		using Splitter = IDragnev::StringSplitter<Containers::DArray>;
		using Container = Containers::DArray<std::string>;

	public:	
		TEST_METHOD(testDefaultSplitterHasOnlyWhiteSpaceDelimiter)
		{
			auto s = Splitter{};

			auto result = s.split("one two 'three'");

			Assert::IsTrue(result == Container{ "one", "two", "'three'"});
		}

		TEST_METHOD(testSplittingTheEmptyStringReturnsEmptyContainer)
		{
			auto s = Splitter{ ' ', '\'' };

			auto result = s.split("");

			Assert::IsTrue(result.isEmpty());
		}

		TEST_METHOD(testWhiteSpacesAreIgnored)
		{
			auto s = Splitter{ '\"', '\'' };

			auto result = s.split("     'one'     ''  \"-two-\"    \"three\"      '*four*'         \"five\"");

			Assert::IsTrue(result == Container{ "one", "", "-two-", "three", "*four*", "five" });
		}

		TEST_METHOD(testWhiteSpaceIsAlwaysConsideredAsDelimiter)
		{
			auto s = Splitter{ '\'' };

			auto result = s.split("     'one'   two   'three and a half'      *four*  ");

			Assert::IsTrue(result == Container{ "one", "two", "three and a half", "*four*" });
		}

		TEST_METHOD(testWhiteSpaceIsMatchedByNullTerminatingCharacter)
		{
			auto s = Splitter{ '\'' };

			auto result = s.split(" one two");

			Assert::IsTrue(result == Container{ "one", "two" });
		}

		TEST_METHOD(testUnmatchedDelimiterThrows)
		{
			auto s = Splitter{ '\'' };

			try
			{
				s.split(" one 'two  ");

				Assert::Fail(L"split did not throw");
			}
			catch (std::runtime_error& e)
			{
				auto message = std::string{ e.what() };
				Assert::IsTrue(message == "Unmatched delimiter: \'");
			}
		}
	};
}