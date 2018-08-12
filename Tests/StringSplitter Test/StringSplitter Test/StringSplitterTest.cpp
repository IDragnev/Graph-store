#include "CppUnitTest.h"
#include "..\..\..\Graph store\Application\StringSplitter\StringSplitter.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringSplitterTest
{		
	TEST_CLASS(StringSplitterTest)
	{
	private:
		typedef StringSplitter<std::vector> Splitter;
		typedef std::vector<std::string> Container;

	public:	
		TEST_METHOD(testSplittingTheEmptyStringReturnsEmptyContainer)
		{
			Splitter splitter;

			Container result = splitter.split("");

			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(testIgnoresDelimiter)
		{
			Splitter splitter;

			Container result = splitter.split("one       two    three  four         five");

			Assert::IsTrue(result == Container{ "one", "two", "three", "four", "five" });
		}
	};
}