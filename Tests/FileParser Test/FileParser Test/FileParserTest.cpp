#include "CppUnitTest.h"
#include "../../../Graph store/File parser/FileParser.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace FileParserTest
{
	TEST_CLASS(FileParserTest)
	{
	private:
		static std::ofstream firstFile;
		static std::ofstream secondFile;
		static const char firstFileName[];
		static const char secondFileName[];
		static const char NEW_LINE = '\n';

		static bool areSame(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static void writeToFirstTestFile(const char* content)
		{
			openTruncated(firstFile, firstFileName);
			writeTo(firstFile, content);
		}

		static void writeToSecondTestFile(const char* content)
		{
			openTruncated(secondFile, secondFileName);
			writeTo(secondFile, content);
		}

		static void openTruncated(std::ofstream& file, const char* name)
		{
			assert(!file.is_open());
			file.open(name, std::ios::out | std::ios::trunc);
			assert(file);
		}

		static void writeTo(std::ofstream& output, const char* content)
		{
			assert(output.is_open());
			output << content;
		}

		static void closeFirstTestFile()
		{
			close(firstFile);
		}

		static void closeSecondTestFile()
		{
			close(secondFile);
		}

		static void close(std::ofstream& output)
		{
			output.close();
		}

	public:
		TEST_METHOD(testDefaultCtorDoesNotOpenAFile)
		{
			FileParser parser;

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(testFilenameCtorWithValidFilenameOpensAFile)
		{
			FileParser parser(firstFileName);

			Assert::IsTrue(parser.hasOpenedFile());
		}

		TEST_METHOD(testOpenFileWithValidFilenameOpensAFile)
		{
			FileParser parser;
			parser.openFile(firstFileName);

			Assert::IsTrue(parser.hasOpenedFile());
		}

		TEST_METHOD(testFilenameCtorWithInvalidFilenameThrows)
		{
			try
			{
				FileParser parser("No-Such-File.txt");

				Assert::Fail(L"Constructor did not throw");
			}
			catch (FileParserException& exception)
			{
				Assert::IsTrue(areSame(exception.what(), "Failed to open No-Such-File.txt"));
			}
		}

		TEST_METHOD(testOpenFileWithInvalidFilenameThrows)
		{
			try
			{
				FileParser parser;

				parser.openFile("No-Such-File.txt");
				Assert::Fail(L"openFile did not throw");
			}
			catch (FileParserException& exception)
			{
				Assert::IsTrue(areSame(exception.what(), "Failed to open No-Such-File.txt"));
			}
		}

		TEST_METHOD(testIgnoreUntilOnEmptyFileReachesEOF)
		{
			FileParser parser(firstFileName);

			parser.ignoreUntil(NEW_LINE);

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(testMoveCtorWithEmptySource)
		{
			FileParser source;
			FileParser destination(std::move(source));

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsFalse(destination.hasOpenedFile(), L"Moved-in object has an associated file");
		}

		TEST_METHOD(testMoveCtorTransfersTheFile)
		{
			FileParser source(firstFileName);
			FileParser destination(std::move(source));

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsTrue(destination.hasOpenedFile(), L"Moved-in object has no associated file");
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			FileParser lhs;
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in object has an associated file");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			FileParser lhs;
			FileParser rhs(firstFileName);

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsTrue(lhs.hasOpenedFile(), L"Moved-in object has no associated file");
		}
	};

	std::ofstream FileParserTest::firstFile;
	std::ofstream FileParserTest::secondFile;
	const char FileParserTest::firstFileName[] = { "FirstFile.txt" };
	const char FileParserTest::secondFileName[] = { "SecondFile.txt" };
}