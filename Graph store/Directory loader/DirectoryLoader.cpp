#include "DirectoryLoader.h"


DirectoryLoader::DirectoryLoader(const String& directory) :
	directory{directory + '\\'},
	iterator{directory},
	builder{}
{
}

 
std::unique_ptr<Graph> DirectoryLoader::loadCurrentFile()
{
	try
	{
		return builder.buildFromFile(getCurrentFileName());
	}
	catch (GraphBuilderException& e)
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