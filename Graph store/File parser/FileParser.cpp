#include "FileParser.h"
#include <string>

namespace IDragnev
{
	namespace GraphStore
	{
		FileParser::ParseFail::ParseFail(const String& filename, const String& reason, std::uint32_t line) :
			Exception{ buildMessage(filename, reason, line) }
		{
		}

		std::string FileParser::ParseFail::buildMessage(const String& filename, const String& reason, std::uint32_t line)
		{
			auto message = std::string{ "Error reading " + filename };
			message += ": ";
			message += reason;
			message += ". Line " + std::to_string(line);

			return message;
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

			stream.open(name);
			if (stream)
			{
				currentLine = 1U;
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

			static const std::size_t bufferSize = 512;
			char buffer[bufferSize];
			stream.getline(buffer, bufferSize);

			throwIfParseFailed("No characters left in the file");

			++currentLine;

			return buffer;
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

			auto extracted = char{};
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
	}
}