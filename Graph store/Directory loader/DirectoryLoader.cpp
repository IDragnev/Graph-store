#include "DirectoryLoader.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include "..\UtilityFunctions.h"
#include "..\Iterator abstraction\Iterator.h"
#include "..\Functional\Functional.h"
#include <iostream>

namespace IDragnev
{
	namespace GraphStore
	{
		DirectoryLoader::DirectoryLoader(const String& path) :
			filesIterator{ path + '\\' }
		{
		}

		void DirectoryLoader::operator()(Function f)
		{
			using PolymorphicRanges::forEach;
			using Functional::compose;

			auto load = [this](const auto& file) { return this->load(file); };
			auto consume = [f](auto result) { if (result != nullptr) f(std::move(result)); };

			forEach(filesIterator, compose(consume, load));
		}

		std::unique_ptr<Graph> DirectoryLoader::load(const String& file)
		{
			try
			{
				return builder(file);
			}
			catch (Exception& e)
			{
				Utility::print(std::cerr, e.what(), "\n");
				return nullptr;
			}
		}
	}
}