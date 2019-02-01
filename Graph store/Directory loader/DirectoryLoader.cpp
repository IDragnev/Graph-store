#include "DirectoryLoader.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include <iostream>

namespace IDragnev
{
	namespace GraphStore
	{
		DirectoryLoader::DirectoryLoader(const String& path) :
			directory{ path + '\\' },
			filesIterator{ directory }
		{
		}

		void DirectoryLoader::operator()(Function fun)
		{
			while (hasRemainingFiles())
			{
				auto result = loadCurrentFile();

				if (result)
				{
					fun(std::move(result));
				}

				goToNextFile();
			}
		}

		std::unique_ptr<Graph> DirectoryLoader::loadCurrentFile()
		{
			try
			{
				return builder.buildFromFile(currentFileName());
			}
			catch (Exception& e)
			{
				std::cerr << e.what() << std::endl;
				return nullptr;
			}
		}

		std::string DirectoryLoader:: currentFileName() const
		{
			return directory + *filesIterator;
		}

		bool DirectoryLoader::hasRemainingFiles() const
		{
			return static_cast<bool>(filesIterator);
		}

		void DirectoryLoader::goToNextFile()
		{
			++filesIterator;
		}
	}
}