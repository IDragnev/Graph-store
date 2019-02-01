#include "CppUnitTest.h"
#include "..\..\..\Graph store\DirectoryTextFilesFlatIterator\DirectoryTextFilesFlatIterator.h"
#include "..\..\..\Graph store\General Exceptions\Exception.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::GraphStore::DirectoryTextFilesFlatIterator;
using IDragnev::GraphStore::Exception;

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

		static const char TEST_DIRECTORY[];
		static const char EMPTY_TEST_DIRECTORY[];
		static const size_t TEST_DIR_TEXT_FILES_COUNT = 3;

		static TextFile textFilesArray[TEST_DIR_TEXT_FILES_COUNT];

	private:
		static bool findsEachTextFileOnce(FlatIterator& iterator)
		{
			while (iterator)
			{
				tryToMatchATextFile(*iterator);
				++iterator;
			}

			return allTextFilesWereMatchedExactlyOnce();
		}

		static void tryToMatchATextFile(const std::string& filename)
		{
			for (auto i = 0U; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
			{
				auto& file = textFilesArray[i];

				if (areEqual(filename.c_str(), file.name))
				{
					++file.timesFound;
				}
			}
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static void markAllTextFilesNotFound()
		{
			for (auto i = 0U; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
			{
				textFilesArray[i].timesFound = 0;
			}
		}

		static bool allTextFilesWereMatchedExactlyOnce()
		{
			for (auto i = 0U; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
			{
				if (textFilesArray[i].timesFound != 1)
				{
					return false;
				}
			}

			return true;
		}

	public:
		TEST_METHOD(testCtorFromInvalidDirThrows)
		{
			try
			{
				FlatIterator iterator{ "Invalid Directory Name" };
				Assert::Fail(L"Constructor did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "No directory with name: Invalid Directory Name"));
			}
		}

		TEST_METHOD(testIteratorToEmptyDirIsFinished)
		{
			FlatIterator iterator{ EMPTY_TEST_DIRECTORY };

			Assert::IsFalse(iterator);
		}

		TEST_METHOD(testIteratorToNonEmptyDir)
		{
			markAllTextFilesNotFound();
			FlatIterator iterator{ TEST_DIRECTORY };

			Assert::IsTrue(iterator, L"Iterator right is finished after construction");
			Assert::IsTrue(findsEachTextFileOnce(iterator), L"Not all text files were found exactly once");
		}
	};

	const char DirectoryFlatIteratorTest::TEST_DIRECTORY[] = { "Test directory" };
	const char DirectoryFlatIteratorTest::EMPTY_TEST_DIRECTORY[] = { "Empty test directory" };
	DirectoryFlatIteratorTest::TextFile DirectoryFlatIteratorTest::textFilesArray[] = { {  "Test directory\\File1.txt", 0 }, {  "Test directory\\File2.txt", 0 }, {  "Test directory\\File3.txt", 0 } };
}