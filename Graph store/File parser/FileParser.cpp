#include "FileParser.h"
#include <string>


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
		
	stream.open(name);
	if (stream)
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
	stream{ std::move(source.stream) }
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
	std::swap(stream, temporary.stream);
}


String FileParser::parseLine()
{
	validateState();

	static const std::size_t BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	stream.getline(buffer, BUFFER_SIZE);

	throwIfParseFailed("No characters left in the file");

	++currentLine;

	return String{ buffer };
}


void FileParser::validateState() const
{
	assert(hasOpenedFile());
	assert(!hasReachedEnd());
	assert(!stream.fail());
}


void FileParser::throwIfParseFailed(const char* reason) const
{
	if (stream.fail())
	{
		throw ParseFail{ filename, reason, currentLine };
	}
}


void FileParser::ignoreUntil(char symbol)
{
	validateState();

	char extracted{};
	do
	{
		extracted = stream.get();

		if (extracted == '\n')
		{
			++currentLine;
		}
	} while (!(hasReachedEnd() || extracted == symbol));
}


char FileParser::peekNextCharacter()
{
	validateState();
	return stream.peek();
}


bool FileParser::hasOpenedFile() const
{
	return stream.is_open();
}


void FileParser::closeFile()
{
	if (hasOpenedFile())
	{
		filename = "";
		currentLine = 0;
		stream.close();
	}
}


bool FileParser::hasReachedEnd() const
{
	assert(hasOpenedFile());
	return stream.eof();
}


char FileParser::endOfFileCharacter()
{
	return EOF;
}


void FileParser::invalidateStreamIfSigned()
{
	if (peekNextCharacter() == '-')
	{
		stream.setstate(stream.rdstate() | std::ios::failbit);
	}
}