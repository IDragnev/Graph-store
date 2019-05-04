#include "FileParser.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "Exceptions\Exceptions.h"
#include <string>

using namespace fmt::literals;

namespace IDragnev::GraphStore
{
	class FailedToOpen : public Exception
	{
	public:
		FailedToOpen::FailedToOpen(const String& filename) :
			Exception{ fmt::format("Failed to open {0} for reading", filename) }
		{
		}
	};

	class NoFileOpened : public Exception
	{
	public:
		NoFileOpened::NoFileOpened() :
			Exception{ "No file associated with parser" }
		{
		}
	};

	class ReachedEnd : public Exception
	{
	public:
		ReachedEnd::ReachedEnd(const String& filename) :
			Exception{ fmt::format("End of {0} reached prematurely", filename) }
		{
		}
	};

	class ParseFail : public Exception
	{
	public:
		ParseFail(const String& filename, const String& reason, std::size_t line) :
			Exception{ fmt::format("Error reading {name}: {reason}! Line {line}.",
								   "name"_a = filename,
								   "reason"_a = reason,
								   "line"_a = line) }
		{
		}
	};

	FileParser::FileParser(const String& filename) :
		FileParser{}
	{
		openFile(filename);
	}

	void FileParser::openFile(const String& filename)
	{
		closeFile();
		open(filename);
	}

	void FileParser::closeFile()
	{
		if (hasOpenedFile())
		{
			filename = "";
			currentLine = 0;
			stream.close();
			stream.clear();
		}
	}

	bool FileParser::hasOpenedFile() const
	{
		return stream.is_open();
	}

	void FileParser::open(const String& file)
	{
		stream.open(file);
		if (stream)
		{
			currentLine = 1u;
			filename = file;
		}
		else
		{
			throw FailedToOpen{ file };
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
			swapContentsWith(std::move(rhs));
		}

		return *this;
	}

	void FileParser::swapContentsWith(FileParser temporary)
	{
		std::swap(currentLine, temporary.currentLine);
		std::swap(filename, temporary.filename);
		std::swap(stream, temporary.stream);
	}

	String FileParser::parseLine()
	{
		validateState();
		
		auto result = doParseLine();
		++currentLine;

		return result;
	}

	String FileParser::doParseLine()
	{
		static const std::size_t bufferSize = 512;
		
		char buffer[bufferSize];
		stream.getline(buffer, bufferSize);
		
		throwIfParseFailed("No characters left in the file");

		return buffer;
	}

	void FileParser::validateState() const
	{
		if (!hasOpenedFile())
		{
			throw NoFileOpened{};
		}
		else if (hasReachedEnd())
		{
			throw ReachedEnd{ filename };
		}
		else if (stream.fail())
		{
			throw Exception{ "Using parser in failed state" };
		}
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

		char extracted;
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

	bool FileParser::hasReachedEnd() const
	{
		if (!hasOpenedFile())
		{
			throw NoFileOpened{};
		}
		
		return stream.eof();
	}

	void FileParser::invalidateStreamIfSigned()
	{
		if (peekNextCharacter() == '-')
		{
			stream.setstate(stream.rdstate() | std::ios::failbit);
		}
	}
}