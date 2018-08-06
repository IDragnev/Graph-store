#ifndef __DIR_ITERATOR_H_INCLUDED__
#define __DIR_ITERATOR_H_INCLUDED__

#include <Windows.h>
#include "..\String\String.h"

class DirectoryIterator
{
public:
	explicit DirectoryIterator(const String& directory);
	~DirectoryIterator();

	bool isFinished() const;
	void goToNextTextFile();
	String getCurrentFileName() const;

private:
	DirectoryIterator(DirectoryIterator&&) = delete;
	DirectoryIterator(const DirectoryIterator&) = delete;
	DirectoryIterator& operator=(DirectoryIterator&&) = delete;
	DirectoryIterator& operator=(const DirectoryIterator&) = delete;

private:
	static const char PATTERN[];

private:
	char path[MAX_PATH];
	HANDLE searchHandle;
	WIN32_FIND_DATA foundFile;
	bool finished;
};

#endif //__DIR_ITERATOR_H_INCLUDED__