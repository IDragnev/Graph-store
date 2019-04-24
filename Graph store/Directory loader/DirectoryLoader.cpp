#include "DirectoryLoader.h"
#include "Graph\Base Graph\Graph.h"
#include "Exceptions\Exceptions.h"
#include "UtilityFunctions.h"
#include "Ranges\Ranges.h"
#include "Functional\Functional.h"
#include "DirectoryTextFilesFlatIterator\DirectoryTextFilesFlatIterator.h"
#include <iostream>

namespace IDragnev::GraphStore
{
	//all members act like local variables and need not be copied
	DirectoryLoader::DirectoryLoader(const DirectoryLoader&) :
		DirectoryLoader{}
	{
	}

	DirectoryLoader& DirectoryLoader::operator=(const DirectoryLoader&)
	{
		return *this;
	}

	void DirectoryLoader::operator()(const String& path, Function f)
	{
		loadFiles(makeDirectoryIterator(path), std::move(f));
	}

	auto DirectoryLoader::makeDirectoryIterator(const String& path) -> DirectoryIterator
	{
		return DirectoryIterator{ path + '\\' };
	}

	void DirectoryLoader::loadFiles(DirectoryIterator iterator, Function f)
	{
		using Ranges::forEach;
		using Functional::compose;

		auto load = [this](const auto& file) { return this->load(file); };
		auto consume = [f](auto result) { if (result != nullptr) f(std::move(result)); };

		forEach(iterator, compose(consume, load));
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