#ifndef __DIR_ITERATOR_H_INCLUDED__
#define __DIR_ITERATOR_H_INCLUDED__

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
			using Path = std::filesystem::path;
			explicit DirectoryTextFilesFlatIterator(const Path& path);
	
			operator bool() const;
			DirectoryTextFilesFlatIterator& operator++();
			std::string operator*() const;

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