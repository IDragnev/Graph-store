#include "CppUnitTest.h"
#include "DirectoryTextFilesFlatIterator\DirectoryTextFilesFlatIterator.h"
#include "String\String.h"
#include "General Exceptions\Exception.h"
#include "Ranges\Ranges.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::GraphStore::DirectoryTextFilesFlatIterator;
using IDragnev::GraphStore::Exception;
using IDragnev::String;

namespace DirectoryFlatIteratorTest
{
	TEST_CLASS(DirectoryFlatIteratorTest)
	{
	private:
		using FlatIterator = DirectoryTextFilesFlatIterator;

		struct TextFile
		{
			const char* const name;
			std::uint8_t timesFound;
		};

	public:
		TEST_METHOD(defaultIteratorIsFinished)
		{
			Assert::IsFalse(FlatIterator{});
		}

		TEST_METHOD(ctorFromInvalidDirectoryThrows)
		{
			try
			{
				FlatIterator{ "Invalid Directory Name" };
				Assert::Fail(L"Constructor did not throw");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(iteratorToEmptyDirectoryIsFinished)
		{
			FlatIterator iterator{ EMPTY_TEST_DIRECTORY };

			Assert::IsFalse(iterator);
		}

		TEST_METHOD(iteratorToNonEmptyDirectory)
		{
			markAllTextFilesNotFound();
			FlatIterator iterator{ TEST_DIRECTORY };

			Assert::IsTrue(iterator, L"Iterator is finished");
			Assert::IsTrue(findsEachTextFileOnce(iterator), L"Not all text files were found exactly once");
		}

	private:
		static void markAllTextFilesNotFound()
		{
			std::for_each(std::begin(textFiles), std::end(textFiles), [](auto& file) { file.timesFound = 0; });
		}

		static bool findsEachTextFileOnce(FlatIterator& it)
		{
			using IDragnev::Ranges::forEach;

			forEach(it, markMatchingFiles);

			return allTextFilesWereMatchedExactlyOnce();
		}

		static void markMatchingFiles(String filename)
		{
			auto markIfMatched = [&filename](auto& file)
			{
				if (areEqual(filename, file.name))
				{
					++file.timesFound;
				}
			};

			std::for_each(std::begin(textFiles), std::end(textFiles), markIfMatched);
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return std::strcmp(lhs, rhs) == 0;
		}

		static bool allTextFilesWereMatchedExactlyOnce()
		{
			auto isFoundExactlyOnce = [](const auto& file) { return file.timesFound == 1; };

			return std::all_of(std::begin(textFiles), std::end(textFiles), isFoundExactlyOnce);
		}

		static const char TEST_DIRECTORY[];
		static const char EMPTY_TEST_DIRECTORY[];
		static TextFile textFiles[3];
	};

	const char DirectoryFlatIteratorTest::TEST_DIRECTORY[] = { "Test directory" };
	const char DirectoryFlatIteratorTest::EMPTY_TEST_DIRECTORY[] = { "Empty test directory" };
	DirectoryFlatIteratorTest::TextFile DirectoryFlatIteratorTest::textFiles[] = { {  "Test directory\\File1.txt", 0 }, {  "Test directory\\File2.txt", 0 }, {  "Test directory\\File3.txt", 0 } };
}