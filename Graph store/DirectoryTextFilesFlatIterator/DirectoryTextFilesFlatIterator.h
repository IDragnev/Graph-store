#ifndef __DIR_ITERATOR_H_INCLUDED__
#define __DIR_ITERATOR_H_INCLUDED__

#include <filesystem>

namespace IDragnev
{
	class String;

	namespace GraphStore
	{
		class DirectoryTextFilesFlatIterator
		{
		private:
			using Iterator = std::filesystem::directory_iterator;
			using Self = DirectoryTextFilesFlatIterator;

		public:
			explicit DirectoryTextFilesFlatIterator(const String& path);
			DirectoryTextFilesFlatIterator(Self&& source) = default;

			Self& operator=(Self&& rhs) = default;

			operator bool() const;
			Self& operator++();
			String operator*() const;

		private:
			void toNextTextFile();

		private:
			Iterator current;
			Iterator end;
		};
	}
}

#endif //__DIR_ITERATOR_H_INCLUDED__