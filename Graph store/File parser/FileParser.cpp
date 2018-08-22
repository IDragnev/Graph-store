#include "FileParser.h"
#include <string>


FileParser::FileParser() :
	currentLine{ 0 },
	filename{},
	file{}
{
}


FileParser::FileParser(const String& filename) :
	FileParser{}
{
	openFile(filename);
}


void FileParser::openFile(const String& name)
{
	if (hasOpenedFile())
	{
		closeFile();
	}
		
	file.open(name);
	if (file)
	{
		currentLine = 1;
		filename = name;
	}
	else
	{
		throw Exception{ "Failed to open " + name + " for reading" };
	}
}


FileParser::FileParser(FileParser&& source) :
	currentLine{ source.currentLine },
	filename{ std::move(source.filename) },
	file{ std::move(source.file) }
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
	std::swap(filename, temporary.filename);
	std::swap(file, temporary.file);
}


String FileParser::parseLine()
{
	assert(hasOpenedFile());
	assert(!hasReachedEnd());
	assert(!file.fail());

	static const std::size_t BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	file.getline(buffer, BUFFER_SIZE);

	throwIfParseFailed("No characters left in the file");

	++currentLine;

	return String{ buffer };
}


void FileParser::throwIfParseFailed(const char* reason) const
{
	if (file.fail())
	{
		throw ParseFail{ filename, reason, currentLine };
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

		if (extracted == '\n')
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
	if (hasOpenedFile())
	{
		filename = "";
		currentLine = 0;
		file.close();
	}
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