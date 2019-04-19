#ifndef __STRING_SPLITTER_H_INCLUDED__
#define __STRING_SPLITTER_H_INCLUDED__

#include "Containers\Dynamic Array\DArray.h"
#include "Functional\Functional.h"
#include <sstream>
#include <utility>
#include <algorithm>

namespace IDragnev
{
	template <template <typename...> typename Container = Containers::DArray,
	          typename Inserter = Functional::BackInserter>
	class StringSplitter
	{
	private:
		static_assert(std::is_default_constructible_v<Container<std::string>>,
			          "StringSplitter requires Container<std::string> to be default constructible");
		static_assert(std::is_nothrow_move_constructible_v<Container<std::string>>,
			          "StringSplitter requires Container<std::string> to be nothrow move constructible");
		static_assert(std::is_default_constructible_v<Inserter>,
			          "StringSplitter requires Inserter to be default constructible");
		static_assert(std::is_invocable_v<Inserter, Container<std::string>&, std::string&&>,
			          "Incompatible Inserter supplied to StringSplitter");

		using Delimiters = Containers::DArray<char>;
		
	public:
		StringSplitter() = default;
		StringSplitter(std::initializer_list<char> delimiters);
		StringSplitter(StringSplitter&& source);
		StringSplitter(const StringSplitter& source);
		~StringSplitter() = default;

		StringSplitter& operator=(StringSplitter&& rhs);
		StringSplitter& operator=(const StringSplitter& rhs);

		Container<std::string> operator()(const std::string& str);

	private:
		void swapDelimsWith(StringSplitter temp);
		void init(const std::string& str);
		void split();
		void skipWhiteSpaces();
		void chooseDelimiter();
		void advanceIfDelimIsNotWhiteSpace();
		void extractWord();
		void insertIfDelimWasMatched(std::string&& word);
		bool delimWasMatched();
		auto makeScopedClear() noexcept;
		
	private:
		std::istringstream stream;
		Container<std::string> result;
		Delimiters delimiters;
		char currentDelim = ' ';
	};
}

#include "StringSplitterImpl.hpp"
#endif //__STRING_SPLITTER_H_INCLUDED__