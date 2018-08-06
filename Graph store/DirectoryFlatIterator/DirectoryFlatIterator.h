#ifndef __DIR_ITERATOR_H_INCLUDED__
#define __DIR_ITERATOR_H_INCLUDED__

#include <Windows.h>
#include "..\String\String.h"

class DirectoryFlatIterator
{
private:
	typedef WIN32_FIND_DATA File;
	typedef HANDLE SearchHandle;

public:
	explicit DirectoryFlatIterator(const String& directory);
	~DirectoryFlatIterator();

	bool isFinished() const;
	void goToNextTextFile();
	String getCurrentFileName() const;

private:
private:
	DirectoryFlatIterator(DirectoryFlatIterator&&) = delete;
	DirectoryFlatIterator(const DirectoryFlatIterator&) = delete;
	DirectoryFlatIterator& operator=(DirectoryFlatIterator&&) = delete;
	DirectoryFlatIterator& operator=(const DirectoryFlatIterator&) = delete;

private:
	static const char PATTERN[];

private:
	SearchHandle handle;
	File foundFile;
	bool finished;
};

#endif //__DIR_ITERATOR_H_INCLUDED__