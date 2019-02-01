#ifndef __DIR_ITERATOR_H_INCLUDED__
#define __DIR_ITERATOR_H_INCLUDED__

#include "..\String\String.h"
#include <filesystem>

namespace IDragnev
{
	namespace GraphStore
	{
		class Exception;

		class DirectoryTextFilesFlatIterator
		{
		private:
			using DirIterator = std::filesystem::directory_iterator;

		public:
			explicit DirectoryTextFilesFlatIterator(const String& path);
	
			operator bool() const;
			DirectoryTextFilesFlatIterator& operator++();
			String operator*() const;

		private:
			void toNextTextFile();
			bool isCurrentEntryATextFile() const;

		private:
			DirIterator iterator;
			DirIterator end;
		};
	}
}

#endif //__DIR_ITERATOR_H_INCLUDED__