#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include "../String/String.h"
#include "FileParserException/FileParserException.h"

class FileParser
{
public:
	FileParser();
	explicit FileParser(const char* filename);
	FileParser(FileParser&& source);
	~FileParser() = default;

	FileParser& operator=(FileParser&& rhs);

	void openFile(const char* filename);
	void closeFile();
	bool hasOpenedFile() const;
	bool hasReachedEnd() const;

	template <typename Integer>
	Integer parseInteger();
	String parseLine();

	void ignoreUntil(char symbol);

private:
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;

	void swapContentsWithReconstructedParameter(FileParser temp);
	void throwIfParseFailed(const char* message) const;

private:
	static const std::streamsize MAX_LIMIT = std::numeric_limits<std::streamsize>::max();
	static const char NEW_LINE = '\n';
	static const size_t BUFFER_SIZE = 512;

private:
	unsigned currentLine;
	char buffer[BUFFER_SIZE];
	std::ifstream file;
};

#include "FileParserImpl.hpp"
#endif //__FILE_PARSER_H_INCLUDED__