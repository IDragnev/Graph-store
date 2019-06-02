#include "DirectoryTextFilesFlatIterator.h"
#include "Exceptions\Exceptions.h"
#include "Functional\Functional.h"
#include "String\String.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "Third party\fmt-5.3.0\include\fmt\ostream.h"
#include <assert.h>
#include <algorithm>

namespace fs = std::filesystem;

namespace IDragnev::GraphStore
{
	class NoSuchDirectory : public Exception
	{
	public:
		NoSuchDirectory(const String& path) :
			Exception{ fmt::format("No directory '{}' was found", path) }
		{
		}
	};

	DirectoryTextFilesFlatIterator::DirectoryTextFilesFlatIterator(const String& path)
	{
		try
		{
			current = Iterator{ static_cast<const char*>(path) };
			toNextTextFile();
		}
		catch (fs::filesystem_error&)
		{
			throw NoSuchDirectory{ path };
		}
	}

	void DirectoryTextFilesFlatIterator::toNextTextFile()
	{
		using Functional::matches;

		auto isTextFile = matches(".txt", [](const auto& entry) { return entry.path().extension(); });
		current = std::find_if(current, end, isTextFile);
	}

	DirectoryTextFilesFlatIterator::operator bool() const
	{
		return current != end;
	}

	String DirectoryTextFilesFlatIterator::operator*() const
	{
		assert(*this);

		auto& entry = *current;
		return entry.path().string().c_str();
	}

	DirectoryTextFilesFlatIterator& DirectoryTextFilesFlatIterator::operator++()
	{
		assert(*this);

		++current;
		toNextTextFile();

		return *this;
	}
}

