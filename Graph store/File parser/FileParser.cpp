#include "FileParser.h"
#include <string>


FileParser::FileParser() :
	currentLine(0),
	file()
{
}


FileParser::FileParser(const char* filename) :
	FileParser()
{
	openFile(filename);
}


void FileParser::openFile(const char* filename)
{
	assert(filename);

	if (hasOpenedFile())
	{
		closeFile();
	}
		
	file.open(filename);
	if (file)
	{
		currentLine = 1;
	}
	else
	{
		throw FileParserException("Failed to open " + String(filename));
	}
}


FileParser::FileParser(FileParser&& source) :
	currentLine(source.currentLine),
	file(std::move(source.file))
{
	source.currentLine = 0;
}


FileParser& FileParser::operator=(FileParser&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWithReconstructedParameter(std::move(rhs));
	}

	return *this;
}


void FileParser::swapContentsWithReconstructedParameter(FileParser temporary)
{
	std::swap(currentLine, temporary.currentLine);
	std::swap(file, temporary.file);
}


String FileParser::parseLine()
{
	assert(hasOpenedFile());
	assert(!hasReachedEnd());
	assert(!file.fail());

	file.getline(buffer, BUFFER_SIZE);

	throwIfParseFailed("Nothing left to parse");

	++currentLine;

	return String(buffer);
}


void FileParser::throwIfParseFailed(const char* message) const
{
	if (file.fail())
	{
		std::string suffix = " at line " + std::to_string(currentLine);
		throw FileParserException(message + suffix);
	}
}


void FileParser::ignoreUntil(char symbol)
{
	assert(hasOpenedFile());
	assert(!hasReachedEnd());
	assert(!file.fail());

	char extracted;
	do
	{
		extracted = file.get();

		if (extracted == NEW_LINE)
		{
			++currentLine;
		}
	} while (!(hasReachedEnd() || extracted == symbol));
}


char FileParser::peekNextCharacter()
{
	assert(hasOpenedFile());
	assert(!hasReachedEnd());
	assert(!file.fail());

	return file.peek();
}


bool FileParser::hasOpenedFile() const
{
	return file.is_open();
}


void FileParser::closeFile()
{
	file.close();
}


bool FileParser::hasReachedEnd() const
{
	assert(hasOpenedFile());

	return file.eof();
}


char FileParser::endOfFileCharacter()
{
	return EOF;
}