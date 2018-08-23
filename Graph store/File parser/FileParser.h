#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include "..\String\String.h"
#include "ParseFail exception\ParseFail.h"

class FileParser
{
public:
	FileParser() = default;
	explicit FileParser(const String& filename);
	FileParser(FileParser&& source);
	~FileParser() = default;

	FileParser& operator=(FileParser&& rhs);

	static char endOfFileCharacter();

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

private:
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;

	void swapContentsWithReconstructedParameter(FileParser temp);

	template <typename ArithmeticType>
	ArithmeticType parseArithmeticType();
	void invalidateStreamIfSigned();
	void throwIfParseFailed(const char* reason) const;
	void validateState() const;

private:
	unsigned currentLine{};
	String filename;
	std::ifstream stream;
};

#include "FileParserImpl.hpp"
#endif //__FILE_PARSER_H_INCLUDED__