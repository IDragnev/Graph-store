#ifndef __STRING_SPLITTER_H_INCLUDED__
#define __STRING_SPLITTER_H_INCLUDED__

#include <sstream>
#include <utility>
#include "..\..\Dynamic Array\DArray.h"
#include "..\..\General Exceptions\Exception.h"

template <template <typename...> typename Container = DArray>
class StringSplitter
{
private:
	static_assert(std::is_default_constructible<Container<std::string>>::value,
				  "StringSplitter requires Container<std::string> to be default constructible");
	static_assert(std::is_move_constructible<Container<std::string>>::value, 
				  "StringSplitter requires Container<std::string> to be move constructible");
	static_assert(std::is_move_assignable<Container<std::string>>::value, 
				  "StringSplitter requires Container<std::string> to be move assignable");

public:
	StringSplitter() = default;
	StringSplitter(std::initializer_list<char> delimiters);
	StringSplitter(const StringSplitter<Container>&) = delete;
	StringSplitter(StringSplitter<Container>&& source) = default;
	
	StringSplitter& operator=(const StringSplitter<Container>&) = delete;
	StringSplitter& operator=(StringSplitter<Container>&& rhs) = default;

	Container<std::string> split(const std::string& str);

private:
	void init(const std::string& str);
	void split();
	void skipWhiteSpaces();
	void chooseDelimiter();
	void advanceIfDelimIsNotWhiteSpace();
	void extractWord();
	void insertIfDelimWasMatched(std::string&& word);
	bool wasDelimMatched();

private:
	std::istringstream stream;
	Container<std::string> result;
	DArray<char> delimiters;
	char currentDelim = ' ';
};

#include "StringSplitterImpl.hpp"
#endif //__STRING_SPLITTER_H_INCLUDED__