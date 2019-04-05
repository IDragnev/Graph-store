#ifndef __STRING_SPLITTER_H_INCLUDED__
#define __STRING_SPLITTER_H_INCLUDED__

#include "..\Containers\Dynamic Array\DArray.h"
#include "..\Functional\Functional.h"
#include <sstream>
#include <utility>
#include <algorithm>

namespace IDragnev
{
	template <template <typename...> typename Container = Containers::DArray>
	class StringSplitter
	{
	private:
		static_assert(std::is_default_constructible_v<Container<std::string>>,
					  "StringSplitter requires Container<std::string> to be default constructible");
		static_assert(std::is_move_constructible_v<Container<std::string>>,
					  "StringSplitter requires Container<std::string> to be move constructible");
		static_assert(std::is_move_assignable_v<Container<std::string>>,
					  "StringSplitter requires Container<std::string> to be move assignable");

		using Delimiters = Containers::DArray<char>;

	public:
		StringSplitter() = default;
		StringSplitter(std::initializer_list<char> delimiters);
		StringSplitter(const StringSplitter&) = delete;
		StringSplitter(StringSplitter&& source) = default;

		StringSplitter& operator=(const StringSplitter&) = delete;
		StringSplitter& operator=(StringSplitter&& rhs) = default;

		Container<std::string> operator()(const std::string& str);

	private:
		void init(const std::string& str);
		void split();
		void skipWhiteSpaces();
		void chooseDelimiter();
		void advanceIfDelimIsNotWhiteSpace();
		void extractWord();
		void insertIfDelimWasMatched(std::string&& word);
		bool delimWasMatched();

	private:
		std::istringstream stream;
		Container<std::string> result;
		Delimiters delimiters;
		char currentDelim = ' ';
	};
}

#include "StringSplitterImpl.hpp"
#endif //__STRING_SPLITTER_H_INCLUDED__