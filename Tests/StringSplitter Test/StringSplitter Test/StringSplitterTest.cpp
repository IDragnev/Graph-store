#include "CppUnitTest.h"
#include "..\..\..\Graph store\StringSplitter\StringSplitter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace IDragnev::Containers;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		using Splitter = IDragnev::StringSplitter<DArray>;
		using Container = DArray<std::string>;

	public:	
		TEST_METHOD(defaultSplitterHasOnlyWhiteSpaceDelimiter)
		{
			Splitter s;

			auto result = s.split("one two 'three'");
			
			Assert::IsTrue(result == Container{ "one", "two", "'three'"});
		}

		TEST_METHOD(splittingTheEmptyStringReturnsEmptyContainer)
		{
			auto s = Splitter{ ' ', '\'' };

			auto result = s.split("");

			Assert::IsTrue(result.isEmpty());
		}

		TEST_METHOD(whiteSpacesAreIgnored)
		{
			auto s = Splitter{ '\"', '\'' };

			auto result = s.split("     'one'     ''  \"-two-\"    \"three\"      '*four*'         \"five\"");

			Assert::IsTrue(result == Container{ "one", "", "-two-", "three", "*four*", "five" });
		}

		TEST_METHOD(whiteSpaceIsAlwaysConsideredAsDelimiter)
		{
			auto s = Splitter{ '\'' };

			auto result = s.split("     'one'   two   'three and a half'      *four*  ");

			Assert::IsTrue(result == Container{ "one", "two", "three and a half", "*four*" });
		}

		TEST_METHOD(whiteSpaceIsMatchedByNullTerminatingCharacter)
		{
			auto s = Splitter{ '\'' };

			auto result = s.split(" one two");

			Assert::IsTrue(result == Container{ "one", "two" });
		}

		TEST_METHOD(unmatchedDelimiterThrows)
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