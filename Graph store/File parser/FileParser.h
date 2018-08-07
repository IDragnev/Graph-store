#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include "../String/String.h"
#include "../Exception/Exception.h"

class FileParser;
typedef Exception<FileParser> FileParserException;

class FileParser
{
public:
	FileParser();
	explicit FileParser(const String& filename);
	FileParser(FileParser&& source);
	~FileParser() = default;

	FileParser& operator=(FileParser&& rhs);

	static char endOfFileCharacter();

	void openFile(const String& name);
	void closeFile();
	bool hasOpenedFile() const;
	bool hasReachedEnd() const;

	template <typename Integer>
	Integer parseInteger();
	String parseLine();

	void ignoreUntil(char symbol);
	char peekNextCharacter();

private:
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;

	void swapContentsWithReconstructedParameter(FileParser temp);
	void throwIfParseFailed(const char* reason) const;

private:
	static const std::streamsize MAX_LIMIT = std::numeric_limits<std::streamsize>::max();
	static const size_t BUFFER_SIZE = 512;

private:
	unsigned currentLine;
	String filename;
	std::ifstream file;
};

#include "FileParserImpl.hpp"
#endif //__FILE_PARSER_H_INCLUDED__