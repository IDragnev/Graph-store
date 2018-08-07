#include "CppUnitTest.h"
#include "../../../Graph store/DirectoryFlatIterator/DirectoryFlatIterator.h"
#include "../../../Graph store/Dynamic Array/DArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DirectoryFlatIteratorTest
{
	TEST_CLASS(DirectoryFlatIteratorTest)
	{
	private:
		static const char TEST_DIRECTORY[];
		static const char EMPTY_TEST_DIRECTORY[];
		static const size_t TEST_DIR_TEXT_FILES_COUNT = 3;
		static const char* TEST_DIR_TEXT_FILES_NAMES[];

		static bool iteratesAllTextFiles(DirectoryFlatIterator& iterator)
		{
			unsigned iteratedFiles = 0;

			while (!iterator.isFinished())
			{
				++iteratedFiles;

				if (!matchesATextFile(iterator.getCurrentFileName()))
				{
					return false;
				}

				iterator.goToNextTextFile();
			}

			return iteratedFiles == TEST_DIR_TEXT_FILES_COUNT;
		}

		static bool matchesATextFile(const String& filename)
		{
			for (size_t i = 0; i < TEST_DIR_TEXT_FILES_COUNT; ++i)
			{
				if (areEqual(filename, TEST_DIR_TEXT_FILES_NAMES[i]))
				{
					return true;
				}
			}

			return false;
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

	public:
		TEST_METHOD(testCtorFromInvalidDirThrows)
		{
			try
			{
				DirectoryFlatIterator iterator("Invalid Directory Name");
				Assert::Fail(L"Constructor did not throw");
			}
			catch (std::runtime_error& exception)
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
			DirectoryFlatIterator iterator(TEST_DIRECTORY);

			Assert::IsFalse(iterator.isFinished(), L"Iterator to non-empty directory is finished after construction");
			Assert::IsTrue(iteratesAllTextFiles(iterator), L"Iterator does not iterate all the text files in the directory");
		}
	};

	const char DirectoryFlatIteratorTest::TEST_DIRECTORY[] = { "Test directory" };
	const char DirectoryFlatIteratorTest::EMPTY_TEST_DIRECTORY[] = { "Empty test directory" };
	const char* DirectoryFlatIteratorTest::TEST_DIR_TEXT_FILES_NAMES[] = { "File1.txt", "File2.txt", "File3.txt" };
}