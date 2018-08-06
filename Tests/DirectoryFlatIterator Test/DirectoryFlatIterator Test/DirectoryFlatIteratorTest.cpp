#include "CppUnitTest.h"
#include "../../../Graph store/DirectoryFlatIterator/DirectoryFlatIterator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DirectoryFlatIteratorTest
{
	TEST_CLASS(DirectoryFlatIteratorTest)
	{
	private:
		static const char TEST_DIRECTORY[];
		static const char EMPTY_TEST_DIRECTORY[];
		static const size_t TEST_DIR_TEXT_FILES_COUNT = 3;
		
		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static bool iteratesAllFiles(DirectoryFlatIterator& iterator)
		{

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
			//Assert::IsTrue(iteratesAllFiles(iterator), L"Iterator does not iterate all the files in the directory");
		}
	};

	const char DirectoryFlatIteratorTest::TEST_DIRECTORY[] = { "D:\\Graph store\\Tests\\DirectoryFlatIterator Test\\Text files" };
	const char DirectoryFlatIteratorTest::EMPTY_TEST_DIRECTORY[] = { "D:\\Graph store\\Tests\\DirectoryFlatIterator Test\\Empty Dir" };
}