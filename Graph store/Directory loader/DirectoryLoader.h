#ifndef __DIRECTORY_LOADER_H_INCLUDED__
#define __DIRECTORY_LOADER_H_INCLUDED__

#include "..\DirectoryTextFilesFlatIterator\DirectoryTextFilesFlatIterator.h"
#include "..\Serialization\Graph Builder\GraphBuilder.h"
#include <functional>
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		class Graph;

		class DirectoryLoader
		{
		private:
			using Function = std::function<void(std::unique_ptr<Graph>)>;
			using FlatIterator = DirectoryTextFilesFlatIterator;

		public:
			explicit DirectoryLoader(const String& path);
			~DirectoryLoader() = default;

			DirectoryLoader(const DirectoryLoader&) = delete;
			DirectoryLoader& operator=(const DirectoryLoader&) = delete;

			void operator()(Function consumer);

		private:
			std::unique_ptr<Graph> load(String&& file);

		private:
			FlatIterator filesIterator;
			GraphBuilder builder;
		};
	}
}

#endif //__DIRECTORY_LOADER_H_INCLUDED__
