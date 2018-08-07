#include "CppUnitTest.h"
#include "../../../Graph store/DirectoryFlatIterator/DirectoryFlatIterator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DirectoryFlatIteratorTest
{
	TEST_CLASS(DirectoryFlatIteratorTest)
	{
	private:
		struct TextFile
		{
			const char* const name;
			unsigned short timesFound;
		};

		static const char TEST_DIRECTORY[];
		static const char EMPTY_TEST_DIRECTORY[];
		static const size_t TEST_DIR_TEXT_FILES_COUNT = 3;

		static TextFile textFilesArray[TEST_DIR_TEXT_FILES_COUNT];

	private:
		static bool findsEachTextFileOnce(DirectoryFlatIterator& iterator)
		{
			while (!iterator.isFinished())
			{
				tryToMatchATextFile(iterator.getCurrentTextFileName());

				iterator.goToNextTextFile();
			}

			return allTextFilesWereMatchedExactlyOnce();
		}

		static void tryToMatchATextFile(const String& filename)
		{
			for (size_t i = 0; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
			{
				TextFile& file = textFilesArray[i];

				if (areEqual(filename, file.name))
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
			for (size_t i = 0; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
			{
				textFilesArray[i].timesFound = 0;
			}
		}

		static bool allTextFilesWereMatchedExactlyOnce()
		{
			for (size_t i = 0; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
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
				DirectoryFlatIterator iterator("Invalid Directory Name");
				Assert::Fail(L"Constructor did not throw");
			}
			catch (DirectoryFlatIteratorException& exception)
			{
				Assert::IsTrue(areEqual(exception.what(), "Failed to open: Invalid Directory Name"));
			}
		}

		TEST_METHOD(testIteratorToEmptyDirIsFinished)
		{
			DirectoryFlatIterator iterator(EMPTY_TEST_DIRECTORY);

			Assert::IsTrue(iterator.isFinished());
		}

		TEST_METHOD(testIteratorToNonEmptyDir)
		{
			markAllTextFilesNotFound();
			DirectoryFlatIterator iterator(TEST_DIRECTORY);

			Assert::IsFalse(iterator.isFinished(), L"Iterator to non-empty directory is finished after construction");
			Assert::IsTrue(findsEachTextFileOnce(iterator), L"Iterator does not find all the text files in the directory exactly once");
		}
	};

	const char DirectoryFlatIteratorTest::TEST_DIRECTORY[] = { "Test directory" };
	const char DirectoryFlatIteratorTest::EMPTY_TEST_DIRECTORY[] = { "Empty test directory" };
	DirectoryFlatIteratorTest::TextFile DirectoryFlatIteratorTest::textFilesArray[] = { { "File1.txt", 0 }, { "File2.txt", 0 }, { "File3.txt", 0 } };
}