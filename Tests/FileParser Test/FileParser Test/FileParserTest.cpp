#include "CppUnitTest.h"
#include "File parser\FileParser.h"
#include "Exceptions\Exceptions.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::GraphStore::FileParser;
using IDragnev::String;
using IDragnev::GraphStore::Exception;

namespace FileParserTest
{
	TEST_CLASS(FileParserTest)
	{
	public:
		TEST_METHOD(defaultParserOpensNoFile)
		{
			FileParser parser;

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(usingParserWithNoAssociatedFileThrows)
		{
			try
			{
				FileParser{}.parseLine();
				Assert::Fail(L"parser did not throw");
			}
			catch(Exception&)
			{
			}
		}

		TEST_METHOD(openingAFileWithConstructor)
		{
			writeToFirstFile("1");

			FileParser parser{ FIRST_FILE_NAME };

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"Incorrect contents read");
		}

		TEST_METHOD(openingAValidFile)
		{
			writeToFirstFile("1");

			FileParser parser;
			parser.openFile(FIRST_FILE_NAME);

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
		}

		TEST_METHOD(openingNonExistingFileWithCtor)
		{
			try
			{
				FileParser parser{ "No-Such-File.txt" };

				Assert::Fail(L"Constructor did not throw");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(openingNonExistingFile)
		{
			FileParser parser;

			try
			{
				parser.openFile("No-Such-File.txt");
				Assert::Fail(L"openFile did not throw");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(ignoreUntilOnMissingSymbolReachesEndOfFile)
		{
			writeToFirstFile("12345");
			FileParser parser{ FIRST_FILE_NAME };

			parser.ignoreUntil('c');

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(ignoreUntilStopsCorrectly)
		{
			writeToFirstFile("12331");

			FileParser parser{ FIRST_FILE_NAME };

			parser.ignoreUntil('3');
			Assert::AreEqual(parser.peekNextCharacter(), '3');
		}

		TEST_METHOD(moveCtorWithEmptySource)
		{
			FileParser source;
			FileParser destination{ std::move(source) };

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(destination.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(moveCtorWithNonEmptySource)
		{
			writeToFirstFile("1");
			FileParser source{ FIRST_FILE_NAME };

			FileParser destination{ std::move(source) };

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsTrue(destination.hasOpenedFile(), L"Moved-in parser has no associated file");
			Assert::AreEqual(destination.peekNextCharacter(), '1', L"The moved file is incorrect");
		}

		TEST_METHOD(moveAssignmentEmptyToEmpty)
		{
			FileParser lhs;
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(MoveAssignmentNonEmptyToEmpty)
		{
			writeToFirstFile("1");
			FileParser rhs{ FIRST_FILE_NAME };
			FileParser lhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsTrue(lhs.hasOpenedFile(), L"Moved-in parser has no associated file");
			Assert::AreEqual(lhs.peekNextCharacter(), '1', L"The moved file is incorrect");
		}

		TEST_METHOD(moveAssignmentEmptyToNonEmpty)
		{
			writeToFirstFile("1");
			FileParser lhs{ FIRST_FILE_NAME };
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(moveAssignmentNonEmptyToNonEmpty)
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

		TEST_METHOD(parsingUnsigned)
		{
			writeToFirstFile("1");
			FileParser parser{ FIRST_FILE_NAME };

			auto result = parser.parseUnsigned();

			Assert::AreEqual(result, 1u, L"Incorrect result");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end of file after parsing the last character");
		}

		TEST_METHOD(parsingSigned)
		{
			writeToFirstFile("-1");
			FileParser parser{ FIRST_FILE_NAME };

			auto result = parser.parseSigned();

			Assert::AreEqual(result, -1, L"Incorrect result");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end of file after parsing the last character");
		}

		TEST_METHOD(parsingALine)
		{
			writeToFirstFile("Line");
			FileParser parser{ FIRST_FILE_NAME };

			auto result = parser.parseLine();

			Assert::IsTrue(areEqual(result, "Line"), L"Parsed line is incorrect");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end after parsing the last line");
		}

		TEST_METHOD(parsingTheEmptyString)
		{
			writeToFirstFile("\nLine 2");
			FileParser parser{ FIRST_FILE_NAME };

			auto result = parser.parseLine();

			Assert::IsTrue(areEqual(result, ""));
		}

		TEST_METHOD(parsingAfterEndOfFileThrows)
		{
			FileParser parser{ FIRST_FILE_NAME };
			parser.ignoreUntil(FileParser::END_OF_FILE);

			try
			{
				auto result = parser.parseLine();
				Assert::Fail(L"parser did not throw");
			}
			catch (Exception&)
			{
			}
		}

		TEST_METHOD(parsingUnsignedFromCharacterThrows)
		{
			testParsingInvalidInput(&FileParser::parseUnsigned<unsigned>, "c");
		}

		TEST_METHOD(parsingSignedFromCharacterThrows)
		{
			testParsingInvalidInput(&FileParser::parseSigned<int>, "c");
		}
		TEST_METHOD(parsingUnsignedWithNothingToParseThrows)
		{
			testParsingInvalidInput(&FileParser::parseUnsigned<unsigned>, "");
		}

		TEST_METHOD(parsingSignedWithNothingToParseThrows)
		{
			testParsingInvalidInput(&FileParser::parseSigned<int>, "");
		}
		
		TEST_METHOD(parsingLineWithNothingToParseThrows)
		{
			testParsingInvalidInput(&FileParser::parseLine, "");
		}

private:
		template <typename R>
		static void testParsingInvalidInput(R(FileParser::* parse)(), const char* input)
		{
			writeToFirstFile(input);
			FileParser p{ FIRST_FILE_NAME };

			try
			{
				auto result = std::invoke(parse, p);
				Assert::Fail(L"parser did not throw");
			}
			catch (Exception&)
			{
			}
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return std::strcmp(lhs, rhs) == 0;
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

		static std::ofstream firstFile;
		static std::ofstream secondFile;
		static const char FIRST_FILE_NAME[];
		static const char SECOND_FILE_NAME[];
	};

	std::ofstream FileParserTest::firstFile;
	std::ofstream FileParserTest::secondFile;
	const char FileParserTest::FIRST_FILE_NAME[]{ "FirstFile.txt" };
	const char FileParserTest::SECOND_FILE_NAME[]{ "SecondFile.txt" };
}