#include "DirectoryFlatIterator.h"
#include "..\General Exceptions\Exception.h"
#include <assert.h>

namespace GS = IDragnev::GraphStore;

const IDragnev::String GS::DirectoryFlatIterator::PATTERN = "*.txt";

namespace IDragnev
{
	namespace GraphStore
	{
		DirectoryFlatIterator::DirectoryFlatIterator(const String& directory)
		{
			findFirstFile(directory);
		}

		void DirectoryFlatIterator::findFirstFile(const String& directory)
		{
			auto fullPath = appendPatternTo(directory);
			handle = FindFirstFile(fullPath, &foundFile);
			throwIfFailedToOpen(directory);
		}

		String DirectoryFlatIterator::appendPatternTo(const String& directory)
		{
			return directory + '\\' + PATTERN;
		}

		void DirectoryFlatIterator::throwIfFailedToOpen(const String& directory) const
		{
			if (!handleIsValid() && GetLastError() != ERROR_FILE_NOT_FOUND)
			{
				throw Exception{ "Failed to open: " + directory };
			}
		}

		bool DirectoryFlatIterator::handleIsValid() const
		{
			return handle != INVALID_HANDLE_VALUE;
		}

		DirectoryFlatIterator::~DirectoryFlatIterator()
		{
			if (handleIsValid())
			{
				closeHandle();
			}
		}

		void DirectoryFlatIterator::closeHandle()
		{
			assert(handleIsValid());
			FindClose(handle);
		}

		void DirectoryFlatIterator::goToNextTextFile()
		{
			if (!isFinished())
			{
				auto foundAFile = bool(FindNextFile(handle, &foundFile));

				if (!foundAFile)
				{
					finish();
				}
			}
		}

		bool DirectoryFlatIterator::isFinished() const
		{
			return !handleIsValid();
		}

		void DirectoryFlatIterator::finish()
		{
			closeHandle();
			invalidateHandle();
		}

		void DirectoryFlatIterator::invalidateHandle()
		{
			handle = INVALID_HANDLE_VALUE;
		}

		String DirectoryFlatIterator::getCurrentTextFileName() const
		{
			assert(!isFinished());
			return foundFile.cFileName;
		}
	}
}
