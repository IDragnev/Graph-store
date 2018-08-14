#ifndef __DIRECTORY_LOADER_H_INCLUDED__
#define __DIRECTORY_LOADER_H_INCLUDED__

#include "..\DirectoryFlatIterator\DirectoryFlatIterator.h"
#include "..\Graph Builder\GraphBuilder.h"

class DirectoryLoader
{
public:
	explicit DirectoryLoader(const String& directory);
	~DirectoryLoader() = default;

	template <typename Function>
	void load(const Function& fun);

private:
	DirectoryLoader(DirectoryLoader&&) = delete;
	DirectoryLoader(const DirectoryLoader&&) = delete;
	DirectoryLoader& operator=(DirectoryLoader&&) = delete;
	DirectoryLoader& operator=(const DirectoryLoader&) = delete;

	bool hasRemainingFiles() const;
	String getCurrentFileName() const;
	std::unique_ptr<Graph> loadCurrentFile();
	void goToNextFile();

private:
	String directory;
	DirectoryFlatIterator iterator;
	GraphBuilder builder;
};

#include "DirectoryLoaderImpl.hpp"
#endif //__DIRECTORY_LOADER_H_INCLUDED__
