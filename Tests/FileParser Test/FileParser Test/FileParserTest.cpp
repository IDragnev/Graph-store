#include "CppUnitTest.h"
#include "../../../Graph store/File parser/FileParser.h"
#include "..\..\..\Third party\fmt-5.3.0\include\fmt\format.h"
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

		static bool areEqual(const char* lhs, std::string& rhs)
		{
			return areEqual(lhs, rhs.c_str());
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

		static std::string buildErrorMessage(const char* filename, const char* reason, std::size_t line)
		{
			using namespace fmt::literals;
			return fmt::format("Error reading {name}: {reason}! Line {line}.",
								"name"_a = filename,
								"reason"_a = reason,
								"line"_a = line);
		}

	public:
		TEST_METHOD(DefaultCtorDoesNotOpenAFile)
		{
			FileParser parser;

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(CtorWithValidFilenameOpensACorrectFile)
		{
			writeToFirstFile("1");

			FileParser parser{ FIRST_FILE_NAME };

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
		}

		TEST_METHOD(OpenFileWithValidFilenameOpensACorrectFile)
		{
			writeToFirstFile("1");

			auto parser = FileParser{};
			parser.openFile(FIRST_FILE_NAME);

			Assert::IsTrue(parser.hasOpenedFile(), L"The file was not opened");
			Assert::AreEqual(parser.peekNextCharacter(), '1', L"The opened file is incorrect");
		}

		TEST_METHOD(FilenameCtorWithInvalidFilenameThrows)
		{
			try
			{
				FileParser parser{ "No-Such-File.txt" };

				Assert::Fail(L"Constructor did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "Failed to open No-Such-File.txt for reading!"));
			}
		}

		TEST_METHOD(OpenFileWithInvalidFilenameThrows)
		{
			try
			{
				auto parser = FileParser{};

				parser.openFile("No-Such-File.txt");
				Assert::Fail(L"openFile did not throw");
			}
			catch (Exception& e)
			{
				Assert::IsTrue(areEqual(e.what(), "Failed to open No-Such-File.txt for reading!"));
			}
		}

		TEST_METHOD(IgnoreUntilOnMissingSymbolReachesEndOfFile)
		{
			writeToFirstFile("12345");
			auto parser = FileParser{ FIRST_FILE_NAME };

			parser.ignoreUntil('c');

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(IgnoreUntilStopsCorrectly)
		{
			writeToFirstFile("12331");

			auto parser = FileParser{ FIRST_FILE_NAME };

			parser.ignoreUntil('3');
			Assert::AreEqual(parser.peekNextCharacter(), '3');
		}

		TEST_METHOD(MoveCtorWithEmptySource)
		{
			FileParser source;
			FileParser destination{ std::move(source) };

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(destination.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(MoveCtorWithNonEmptySource)
		{
			writeToFirstFile("1");
			FileParser source{ FIRST_FILE_NAME };

			FileParser destination{ std::move(source) };

			Assert::IsFalse(source.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsTrue(destination.hasOpenedFile(), L"Moved-in parser has no associated file");
			Assert::AreEqual(destination.peekNextCharacter(), '1', L"The moved file is incorrect");
		}

		TEST_METHOD(MoveAssignmentEmptyToEmpty)
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

		TEST_METHOD(MoveAssignmentEmptyToNonEmpty)
		{
			writeToFirstFile("1");
			FileParser lhs{ FIRST_FILE_NAME };
			FileParser rhs;

			lhs = std::move(rhs);

			Assert::IsFalse(rhs.hasOpenedFile(), L"Moved-from parser has an associated file");
			Assert::IsFalse(lhs.hasOpenedFile(), L"Moved-in parser has an associated file");
		}

		TEST_METHOD(MoveAssignmentNonEmptyToNonEmpty)
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

		TEST_METHOD(SimpleParseUnsigned)
		{
			writeToFirstFile("1");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseUnsigned();

			Assert::AreEqual(result, 1U, L"Incorrect result");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end of file after parsing the last character");
		}

		TEST_METHOD(SimpleParseSigned)
		{
			writeToFirstFile("-1");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseSigned();

			Assert::AreEqual(result, -1, L"Incorrect result");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end of file after parsing the last character");
		}
		
		TEST_METHOD(SimpleParseLine)
		{
			writeToFirstFile("Line");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseLine();

			Assert::IsTrue(areEqual(result, "Line"), L"Parsed line is incorrect");
			Assert::IsTrue(parser.hasReachedEnd(), L"Parser has not reached the end after parsing the last line");
		}

		TEST_METHOD(ParseLineWithEmptyString)
		{
			writeToFirstFile("\nLine 2");
			auto parser = FileParser{ FIRST_FILE_NAME };

			auto result = parser.parseLine();

			Assert::IsTrue(areEqual(result, ""));
		}

		TEST_METHOD(ParseUnsignedWithCharacterThrows)
		{
			writeToFirstFile("c");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseUnsigned();
				Assert::Fail(L"parseUnsigned() did not throw");
			}
			catch (Exception& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid unsigned number format", 1);
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(ParseSignedWithCharacterThrows)
		{
			writeToFirstFile("c");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseSigned();
				Assert::Fail(L"parseSigned() did not throw");
			}
			catch (Exception& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid signed number format", 1);
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(ParseUnsignedWithSignedThrows)
		{
			writeToFirstFile("-1");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseUnsigned();
				Assert::Fail(L"parseUnsigned() did not throw");
			}
			catch (Exception& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid unsigned number format", 1);
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(ParseUnsignedWithNothingToParseThrows)
		{
			writeToFirstFile("");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseUnsigned();
				Assert::Fail(L"parseUnsigned() did not throw");
			}
			catch (Exception& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid unsigned number format", 1);
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(ParseSignedWithNothingToParseThrows)
		{
			writeToFirstFile("");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseSigned();
				Assert::Fail(L"parseSigned() did not throw");
			}
			catch (Exception& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "Invalid signed number format", 1);
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}

		TEST_METHOD(ParseLineWithNothingToParseThrows)
		{
			writeToFirstFile("");
			auto parser = FileParser{ FIRST_FILE_NAME };

			try
			{
				auto result = parser.parseLine();
				Assert::Fail(L"parseLine() did not throw");
			}
			catch (Exception& e)
			{
				auto expected = buildErrorMessage(FIRST_FILE_NAME, "No characters left in the file", 1);
				Assert::IsTrue(areEqual(e.what(), expected));
			}
		}
	};

	std::ofstream FileParserTest::firstFile;
	std::ofstream FileParserTest::secondFile;
	const char FileParserTest::FIRST_FILE_NAME[] { "FirstFile.txt" };
	const char FileParserTest::SECOND_FILE_NAME[] { "SecondFile.txt" };
}