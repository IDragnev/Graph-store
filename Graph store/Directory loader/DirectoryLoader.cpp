#include "DirectoryLoader.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include <iostream>


DirectoryLoader::DirectoryLoader(const String& directory) :
	directory{ directory + '\\' },
	iterator{ directory },
	builder{}
{
}


void DirectoryLoader::load(Function fun)
{
	while (hasRemainingFiles())
	{
		std::unique_ptr<Graph> result = loadCurrentFile();

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
		return builder.buildFromFile(getCurrentFileName());
	}
	catch (Exception& e)
	{
		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}


String DirectoryLoader::getCurrentFileName() const
{
	return directory + iterator.getCurrentTextFileName();
}


bool DirectoryLoader::hasRemainingFiles() const
{
	return !iterator.isFinished();
}


void DirectoryLoader::goToNextFile()
{
	iterator.goToNextTextFile();
}