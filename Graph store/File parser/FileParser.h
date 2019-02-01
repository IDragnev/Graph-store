#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include "..\String\String.h"
#include "..\General Exceptions\Exception.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class FileParser
		{
		public:
			class ParseFail : public Exception
			{
			public:
				ParseFail(const String& filename, const String& reason, std::uint32_t line);

			private:
				static std::string buildMessage(const String& filename, const String& reason, std::uint32_t line);
			};

		public:
			FileParser() = default;
			explicit FileParser(String filename);
			FileParser(FileParser&& source);
			FileParser(const FileParser&) = delete;

			FileParser& operator=(FileParser&& rhs);
			FileParser& operator=(const FileParser&) = delete;

			static char endOfFileCharacter();

			void openFile(String name);
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

		private:
			void swapContentsWith(FileParser temp);

			template <typename ArithmeticType>
			ArithmeticType parseArithmeticType();
			void invalidateStreamIfSigned();
			void throwIfParseFailed(const char* reason) const;
			void validateState() const;

		private:
			std::uint32_t currentLine{};
			String filename;
			std::ifstream stream;
		};
	}
}

#include "FileParserImpl.hpp"
#endif //__FILE_PARSER_H_INCLUDED__