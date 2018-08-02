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
		static const char emptyFileName[];
		static const char NEW_LINE = '\n';

		static bool areSame(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static void writeToFirstTestFile(const char* content)
		{
			openTruncated(firstFile, firstFileName);
			writeTo(firstFile, content);
			close(firstFile);
		}

		static void writeToSecondTestFile(const char* content)
		{
			openTruncated(secondFile, secondFileName);
			writeTo(secondFile, content);
			close(secondFile);
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

		TEST_METHOD(testFilenameCtorWithValidFilenameOpensACorrectFile)
		{
			writeToFirstTestFile("1");

			FileParser parser(firstFileName);

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
		}

		TEST_METHOD(testOpenFileWithValidFilenameOpensACorrectFile)
		{
			writeToFirstTestFile("1");

			FileParser parser;
			parser.openFile(firstFileName);

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
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
			FileParser parser(emptyFileName);

			parser.ignoreUntil(NEW_LINE);

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(testIgnoreUntilLastSymbolReachesEndOfFile)
		{
			writeToFirstTestFile("12345");

			FileParser parser(firstFileName);

			parser.ignoreUntil('5');
			Assert::IsFalse(parser.hasReachedEnd());
		}

		TEST_METHOD(testIgnoreUntilWithInternalSymbolStopsCorrectly)
		{
			writeToFirstTestFile("123@1");

			FileParser parser(firstFileName);

			parser.ignoreUntil('3');
			Assert::AreEqual(parser.peekNextCharacter(), '@');
		}

		TEST_METHOD(testIgnoreUntilWithDuplicateSymbolsStopsAtFirst)
		{
			writeToFirstTestFile("123321");

			FileParser parser(firstFileName);

			parser.ignoreUntil('3');
			Assert::AreEqual(parser.peekNextCharacter(), '3');
		}

		TEST_METHOD(testMoveCtorWithEmptySource)
		{
			FileParser source;
			FileParser destination(std::move(source));

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsFalse(destination.hasOpenedFile(), L"Moved-in object has an associated file");
		}

		TEST_METHOD(testMoveCtorWithNonEmptySource)
		{
			writeToFirstTestFile("1");
			FileParser source(firstFileName);

			FileParser destination(std::move(source));

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsTrue(destination.hasOpenedFile(), L"Moved-in object has no associated file");
			Assert::AreEqual(destination.peekNextCharacter(), '1', L"The moved file is incorrect");
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
			writeToFirstTestFile("1");
			FileParser rhs(firstFileName);
			FileParser lhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsTrue(lhs.hasOpenedFile(), L"Moved-in object has no associated file");
			Assert::AreEqual(lhs.peekNextCharacter(), '1', L"The moved file is incorrect");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			writeToFirstTestFile("1");
			FileParser lhs(firstFileName);
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in object has an associated file");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			writeToFirstTestFile("1");
			writeToSecondTestFile("2");
			FileParser lhs(firstFileName);
			FileParser rhs(secondFileName);

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from object has an associated file");
			Assert::IsTrue(lhs.hasOpenedFile(), L"Moved-in object has no associated file");
			Assert::AreEqual(lhs.peekNextCharacter(), '2', L"The moved file is incorrect");
		}
	};

	std::ofstream FileParserTest::firstFile;
	std::ofstream FileParserTest::secondFile;
	const char FileParserTest::firstFileName[] = { "FirstFile.txt" };
	const char FileParserTest::secondFileName[] = { "SecondFile.txt" };
	const char FileParserTest::emptyFileName[] = { "EmptyFile.txt" };
}