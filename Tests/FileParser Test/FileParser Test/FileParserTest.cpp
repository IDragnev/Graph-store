#include "CppUnitTest.h"
#include "../../../Graph store/File parser/FileParser.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::GraphStore::FileParser;
using IDragnev::String;
using IDragnev::GraphStore::Exception;

namespace FileParserTest
{
	TEST_CLASS(FileParserTest)
	{
	private:
		static std::ofstream firstFile;
		static std::ofstream secondFile;
		static const char FIRST_FILE_NAME[];
		static const char SECOND_FILE_NAME[];

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static void writeToFirstFile(const char* content)
		{
			openTruncated(firstFile, FIRST_FILE_NAME);
			writeTo(firstFile, content);
			firstFile.close();
		}

		static void writeToSecondFile(const char* content)
		{
			openTruncated(secondFile, SECOND_FILE_NAME);
			writeTo(secondFile, content);
			secondFile.close();
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

		static String buildErrorMessage(const char* filename, const char* reason, char line)
		{
			auto result = String{ "Error reading " };
			result += filename;
			result += ": ";
			result += reason;
			result += ". Line ";
			result += line;

			return result;
		}

	public:
		TEST_METHOD(testDefaultCtorDoesNotOpenAFile)
		{
			FileParser parser;

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(testCtorWithValidFilenameOpensACorrectFile)
		{
			writeToFirstFile("1");

			FileParser parser{ FIRST_FILE_NAME };

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
		}

		TEST_METHOD(testOpenFileWithValidFilenameOpensACorrectFile)
		{
			writeToFirstFile("1");

			auto parser = FileParser{};
			parser.openFile(FIRST_FILE_NAME);

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
		}

		TEST_METHOD(testFilenameCtorWithInvalidFilenameThrows)
		{
			try
			{
				FileParser parser{ "No-Such-File.txt" };

				Assert::Fail(L"Constructor did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "Failed to open No-Such-File.txt for reading"));
			}
		}

		TEST_METHOD(testOpenFileWithInvalidFilenameThrows)
		{
			try
			{
				auto parser = FileParser{};

				parser.openFile("No-Such-File.txt");
				Assert::Fail(L"openFile did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "Failed to open No-Such-File.txt for reading"));
			}
		}

		TEST_METHOD(testIgnoreUntilOnMissingSymbolReachesEndOfFile)
		{
			writeToFirstFile("12345");
			auto parser = FileParser{ FIRST_FILE_NAME };

			parser.ignoreUntil('c');

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(testIgnoreUntilStopsCorrectly)
		{
			writeToFirstFile("12331");

			auto parser = FileParser{ FIRST_FILE_NAME };

			parser.ignoreUntil('3');
			Assert::AreEqual(parser.peekNextCharacter(), '3');
		}

		TEST_METHOD(testMoveCtorWithEmptySource)
		{
			FileParser source;
			FileParser destination{ std::move(source) };

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(destination.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(testMoveCtorWithNonEmptySource)
		{
			writeToFirstFile("1");
			FileParser source{ FIRST_FILE_NAME };

			FileParser destination{ std::move(source) };

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsTrue(destination.hasOpenedFile(), L"Moved-in parser has no associated file");
			Assert::AreEqual(destination.peekNextCharacter(), '1', L"The moved file is incorrect");
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			FileParser lhs;
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			writeToFirstFile("1");
			FileParser rhs{ FIRST_FILE_NAME };
			FileParser lhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsTrue(lhs.hasOpenedFile(), L"Moved-in parser has no associated file");
			Assert::AreEqual(lhs.peekNextCharacter(), '1', L"The moved file is incorrect");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			writeToFirstFile("1");
			FileParser lhs{ FIRST_FILE_NAME };
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			writeToFirstFile("1");
			writeToSecondFile("2");
			FileParser lhs{ FIRST_FILE_NAME };
			FileParser rhs{ SECOND_FILE_NAME };

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsTrue(lhs.hasOpenedFile(), L"Moved-in parser has no associated file");
			Assert::AreEqual(lhs.peekNextCharacter(), '2', L"The moved file is incorrect");
		}

		TEST_METHOD(testSimpleParseUnsigned)
		{
			writeToFirstFile("1");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseUnsigned();

			Assert::AreEqual(result, 1U, L"Incorrect result");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end of file after parsing the last character");
		}

		TEST_METHOD(testSimpleParseSigned)
		{
			writeToFirstFile("-1");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseSigned();

			Assert::AreEqual(result, -1, L"Incorrect result");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end of file after parsing the last character");
		}
		
		TEST_METHOD(testSimpleParseLine)
		{
			writeToFirstFile("Line");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseLine();

			Assert::IsTrue(areEqual(result, "Line"), L"Parsed line is incorrect");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end after parsing the last line");
		}

		TEST_METHOD(testParseLineWithEmptyString)
		{
			writeToFirstFile("\nLine 2");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseLine();

			Assert::IsTrue(areEqual(result, ""));
		}

		TEST_METHOD(testParseUnsignedWithCharacterThrows)
		{
			writeToFirstFile("c");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseUnsigned();
				Assert::Fail(L"parseUnsigned() did not throw");
			}
			catch (FileParser::ParseFail& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid unsigned number format", '1');
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(testParseSignedWithCharacterThrows)
		{
			writeToFirstFile("c");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseSigned();
				Assert::Fail(L"parseSigned() did not throw");
			}
			catch (FileParser::ParseFail& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid signed number format", '1');
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(testParseUnsignedWithSignedThrows)
		{
			writeToFirstFile("-1");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseUnsigned();
				Assert::Fail(L"parseUnsigned() did not throw");
			}
			catch (FileParser::ParseFail& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid unsigned number format", '1');
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(testParseUnsignedWithNothingToParseThrows)
		{
			writeToFirstFile("");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseUnsigned();
				Assert::Fail(L"parseUnsigned() did not throw");
			}
			catch (FileParser::ParseFail& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid unsigned number format", '1');
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(testParseSignedWithNothingToParseThrows)
		{
			writeToFirstFile("");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseSigned();
				Assert::Fail(L"parseSigned() did not throw");
			}
			catch (FileParser::ParseFail& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid signed number format", '1');
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(testParseLineWithNothingToParseThrows)
		{
			writeToFirstFile("");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseLine();
				Assert::Fail(L"parseLine() did not throw");
			}
			catch (FileParser::ParseFail& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "No characters left in the file", '1');
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}
	};

	std::ofstream FileParserTest::firstFile;
	std::ofstream FileParserTest::secondFile;
	const char FileParserTest::FIRST_FILE_NAME[] { "FirstFile.txt" };
	const char FileParserTest::SECOND_FILE_NAME[] { "SecondFile.txt" };
}