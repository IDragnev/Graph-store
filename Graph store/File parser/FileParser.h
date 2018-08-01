#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include "../String/String.h"

class FileParser
{
public:
	FileParser();
	FileParser(const char* fileName);
	FileParser(FileParser&& source);
	~FileParser();

	FileParser& operator=(FileParser&& rhs);

	void openFile(const char* fileName);
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

	void throwIfParseFailed(const char* message) const;

private:
	static const std::streamsize MAX_LIMIT = std::numeric_limits<std::streamsize>::max();
	static const char NEW_LINE = '\n';
	static const size_t BUFFER_SIZE = 512;

private:
	unsigned currentLine;
	char buffer[BUFFER_SIZE];
	std::ifstream sourceFile;
};

#include "FileParserImpl.hpp"
#endif //__FILE_PARSER_H_INCLUDED__