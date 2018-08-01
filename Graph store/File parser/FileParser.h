#ifndef __FILE_PARSER_H_INCLUDED__
#define __FILE_PARSER_H_INCLUDED__

#include <fstream>
#include "../String/String.h"

class FileParser
{
public:
	FileParser(const char* fileName);
	FileParser(FileParser&& source);
	~FileParser();

	FileParser& operator=(FileParser&& rhs);

	template <typename Integer>
	Integer parseIntegerType();
	String parseLine();

	void ignoreUntil(char symbol);
	
	bool hasReachedEnd() const;

private:
	FileParser(const FileParser&) = delete;
	FileParser& operator=(const FileParser&) = delete;

	void throwIfFailed(const String& message) const;

private:
	static const std::streamsize MAX_LIMIT = std::numeric_limits<std::streamsize>::max();
	static const char NEW_LINE = '\n';
	static const size_t BUFFER_SIZE = 512;

private:
	unsigned currentLine;
	char buffer[BUFFER_SIZE];
	std::ifstream sourceFile;
};

#endif //__FILE_PARSER_H_INCLUDED__