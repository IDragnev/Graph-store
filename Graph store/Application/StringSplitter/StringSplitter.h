#ifndef __STRING_SPLITTER_H_INCLUDED__
#define __STRING_SPLITTER_H_INCLUDED__

#include <string>
#include <sstream>
#include <vector>

template <template <class...> class Container = std::vector>
class StringSplitter
{
private:
	static_assert(std::is_default_constructible<Container<std::string>>::value, "StringSplitter requires Container<std::string> to be default constructible");
	static_assert(std::is_move_constructible<Container<std::string>>::value, "StringSplitter requires Container<std::string> to be move constructible");
	static_assert(std::is_move_assignable<Container<std::string>>::value, "StringSplitter requires Container<std::string> to be move assignable");

public:
	StringSplitter() = default;
	StringSplitter(StringSplitter<Container>&& source) = default;
	~StringSplitter() = default;
	
	StringSplitter& operator=(StringSplitter<Container>&& rhs) = default;

	Container<std::string> split(const std::string& string, char delimiter = ' ');

private:
	StringSplitter(const StringSplitter<Container>&) = delete;
	StringSplitter& operator=(const StringSplitter<Container>&) = delete;

	void init(const std::string& string, char delimiter);
	void split();
	void ignoreWhileDelim();

private:
	Container<std::string> result;
	std::istringstream stream;
	char delim;
};

#include "StringSplitterImpl.hpp"
#endif //__STRING_SPLITTER_H_INCLUDED__