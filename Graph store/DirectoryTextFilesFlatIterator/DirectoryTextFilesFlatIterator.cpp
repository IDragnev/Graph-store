#include "DirectoryTextFilesFlatIterator.h"
#include "..\General Exceptions\Exception.h"
#include <assert.h>

namespace IDragnev
{
	namespace GraphStore
	{
		DirectoryTextFilesFlatIterator::DirectoryTextFilesFlatIterator(const String& path)
		{
			try
			{
				iterator = DirIterator{ static_cast<const char*>(path) };
				toNextTextFile();
			}
			catch (std::filesystem::filesystem_error&)
			{
				throw Exception{ "No directory with name: " + path };
			}
		}

		void DirectoryTextFilesFlatIterator::toNextTextFile()
		{
			while (*this && !isCurrentEntryATextFile())
			{
				++iterator;
			}
		}

		bool DirectoryTextFilesFlatIterator::isCurrentEntryATextFile() const
		{
			auto& entry = *iterator;
			return entry.path().extension() == ".txt";
		}

		DirectoryTextFilesFlatIterator::operator bool() const
		{
			return iterator != end;
		}

		String DirectoryTextFilesFlatIterator::operator*() const
		{
			auto& entry = *iterator;
			return entry.path().string().c_str();
		}

		DirectoryTextFilesFlatIterator& DirectoryTextFilesFlatIterator::operator++()
		{
			assert(*this);

			++iterator;
			toNextTextFile();
			
			return *this;
		}
	}
}
