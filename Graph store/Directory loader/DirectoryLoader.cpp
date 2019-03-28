#include "DirectoryLoader.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include "..\UtilityFunctions.h"
#include "..\Iterator abstraction\Iterator.h"
#include <iostream>

namespace IDragnev
{
	namespace GraphStore
	{
		DirectoryLoader::DirectoryLoader(const String& path) :
			filesIterator{ path + '\\' }
		{
		}

		void DirectoryLoader::operator()(Function consume)
		{
			using PolymorphicRanges::forEach;
			
			forEach(filesIterator, [this, consume](String&& file)
			{
				auto result = load(std::move(file));

				if (result != nullptr)
				{
					consume(std::move(result));
				}
			});
		}

		std::unique_ptr<Graph> DirectoryLoader::load(String&& file)
		{
			try
			{
				return builder(std::move(file));
			}
			catch (Exception& e)
			{
				Utility::print(std::cerr, e.what(), "\n");
				return nullptr;
			}
		}
	}
}