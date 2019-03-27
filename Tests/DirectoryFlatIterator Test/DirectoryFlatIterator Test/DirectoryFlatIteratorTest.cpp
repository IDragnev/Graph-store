#include "CppUnitTest.h"
#include "..\..\..\Graph store\DirectoryTextFilesFlatIterator\DirectoryTextFilesFlatIterator.h"
#include "..\..\..\Graph store\General Exceptions\Exception.h"
#include "..\..\..\Graph store\Iterator abstraction\Iterator.h"
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

		TEST_METHOD(iteratorToEmptyDirectoryIsDone)
		{
			FlatIterator iterator{ EMPTY_TEST_DIRECTORY };

			Assert::IsFalse(iterator);
		}

		TEST_METHOD(iteratorToNonEmptyDirectory)
		{
			markAllTextFilesNotFound();
			FlatIterator iterator{ TEST_DIRECTORY };

			Assert::IsTrue(iterator, L"Iterator is finished after construction");
			Assert::IsTrue(findsEachTextFileOnce(iterator), L"Not all text files were found exactly once");
		}

	private:
		static void markAllTextFilesNotFound()
		{
			std::for_each(std::begin(textFiles), std::end(textFiles), [](auto& file) { file.timesFound = 0; });
		}

		static bool findsEachTextFileOnce(FlatIterator& it)
		{
			using IDragnev::PolymorphicRanges::forEach;

			forEach(it, markMatchingFiles);

			return allTextFilesWereMatchedExactlyOnce();
		}

		static void markMatchingFiles(const String& filename)
		{
			auto markIfFound = [&filename](auto& file)
			{
				if (areEqual(filename, file.name))
				{
					++file.timesFound;
				}
			};

			std::for_each(std::begin(textFiles), std::end(textFiles), markIfFound);
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