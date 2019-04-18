#ifndef __DIRECTORY_LOADER_H_INCLUDED__
#define __DIRECTORY_LOADER_H_INCLUDED__

#include "Serialization\Graph Builder\GraphBuilder.h"
#include <functional>
#include <memory>

namespace IDragnev::GraphStore
{
	class DirectoryTextFilesFlatIterator;

	class DirectoryLoader
	{
	private:
		using Function = std::function<void(std::unique_ptr<Graph>)>;
		using DirectoryIterator = DirectoryTextFilesFlatIterator;

	public:
		DirectoryLoader() = default;
		DirectoryLoader(const DirectoryLoader& source);
		~DirectoryLoader() = default;

		DirectoryLoader& operator=(const DirectoryLoader& rhs);

		void operator()(const String& path, Function consumer);

	private:
		void loadFiles(DirectoryIterator iterator, Function consumer);
		DirectoryIterator makeDirectoryIterator(const String& path);
		std::unique_ptr<Graph> load(const String& file);

	private:
		GraphBuilder builder;
	};
}

#endif //__DIRECTORY_LOADER_H_INCLUDED__
