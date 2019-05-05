#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include <type_traits>
#include <assert.h>
#include "String\String.h"

namespace IDragnev::GraphStore
{
	class FileParser
	{
	public:
		FileParser() = default;
		explicit FileParser(const String& filename);
		FileParser(FileParser&& source);
		FileParser(const FileParser&) = delete;

		FileParser& operator=(FileParser&& rhs);
		FileParser& operator=(const FileParser&) = delete;

		void openFile(const String& name);
		void closeFile();
		bool hasOpenedFile() const;
		bool hasReachedEnd() const;

		template <typename ArithmeticType = int>
		ArithmeticType parseSigned();
		template <typename ArithmeticType = unsigned>
		ArithmeticType parseUnsigned();
		String parseLine();

		void ignoreUntil(char symbol);
		char peekNextCharacter();

		static const char END_OF_FILE = EOF;

	private:
		void swapContentsWith(FileParser temp);

		void open(const String& filename);
		String doParseLine();

		template <typename ArithmeticType, typename Callable>
		ArithmeticType parseArithmeticType(const char* messageOnFail, Callable prepare);
		template <typename ArithmeticType>
		ArithmeticType parseArithmeticType();
		void invalidateStreamIfSigned();
		void throwIfParseFailed(const char* reason) const;
		void validateState() const;

	private:
		std::size_t currentLine = 0;
		String filename;
		std::ifstream stream;
	};
}

#include "FileParserImpl.hpp"
#endif //__FILE_PARSER_H_INCLUDED__