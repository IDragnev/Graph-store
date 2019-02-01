#include "DirectoryLoader.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include <iostream>

namespace IDragnev
{
	namespace GraphStore
	{
		DirectoryLoader::DirectoryLoader(const String& path) :
			filesIterator{ path + '\\' }
		{
		}

		void DirectoryLoader::operator()(Function consumer)
		{
			while (filesIterator)
			{
				auto result = load(*filesIterator);

				if (result != nullptr)
				{
					consumer(std::move(result));
				}

				++filesIterator;
			}
		}

		std::unique_ptr<Graph> DirectoryLoader::load(String&& file)
		{
			try
			{
				return builder.buildFromFile(std::move(file));
			}
			catch (Exception& e)
			{
				std::cerr << e.what() << std::endl;
				return nullptr;
			}
		}
	}
}