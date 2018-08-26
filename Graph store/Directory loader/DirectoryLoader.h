#ifndef __DIRECTORY_LOADER_H_INCLUDED__
#define __DIRECTORY_LOADER_H_INCLUDED__

#include "..\DirectoryFlatIterator\DirectoryFlatIterator.h"
#include "..\Graph Builder\GraphBuilder.h"
#include "..\String\String.h"
#include <functional>
#include <memory>

class Graph;

class DirectoryLoader
{
private:
	using Function = std::function<void(std::unique_ptr<Graph>)>;

public:
	explicit DirectoryLoader(const String& directory);

	void load(Function f);

private:
	DirectoryLoader(DirectoryLoader&&) = delete;
	DirectoryLoader(const DirectoryLoader&) = delete;
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

#endif //__DIRECTORY_LOADER_H_INCLUDED__
