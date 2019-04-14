
namespace IDragnev
{
	template <template <typename...> typename Container, typename Inserter>
	StringSplitter<Container, Inserter>::StringSplitter(std::initializer_list<char> delimiters) :
		delimiters{ delimiters }
	{
	}

	template <template <typename...> typename Container, typename Inserter>
	Container<std::string> StringSplitter<Container, Inserter>::operator()(const std::string& str)
	{
		init(str);
		split();

		return std::move(result);
	}

	template <template <typename...> typename Container, typename Inserter>
	inline void StringSplitter<Container, Inserter>::init(const std::string& str)
	{
		stream.clear();
		stream.str(str);
	}

	template <template <typename...> typename Container, typename Inserter>
	void StringSplitter<Container, Inserter>::split()
	{
		do
		{
			skipWhiteSpaces();
			chooseDelimiter();
			advanceIfDelimIsNotWhiteSpace();
			extractWord();
		} while (stream.good());
	}

	template <template <typename...> typename Container, typename Inserter>
	void StringSplitter<Container, Inserter>::skipWhiteSpaces()
	{
		while (stream.peek() == ' ')
		{
			stream.ignore();
		}
	}

	template <template <typename...> typename Container, typename Inserter>
	void StringSplitter<Container, Inserter>::chooseDelimiter()
	{
		using Functional::equalTo;

		auto it = std::find_if(std::cbegin(delimiters), std::cend(delimiters), equalTo(stream.peek()));
		currentDelim = it ? *it : ' ';
	}

	template <template <typename...> typename Container, typename Inserter>
	inline void StringSplitter<Container, Inserter>::advanceIfDelimIsNotWhiteSpace()
	{
		if (currentDelim != ' ')
		{
			stream.ignore();
		}
	}

	template <template <typename...> typename Container, typename Inserter>
	void StringSplitter<Container, Inserter>::extractWord()
	{
		auto word = std::string{};
		std::getline(stream, word, currentDelim);

		if (!stream.fail())
		{
			insertIfDelimWasMatched(std::move(word));
		}
	}

	template <template <typename...> typename Container, typename Inserter>
	void StringSplitter<Container, Inserter>::insertIfDelimWasMatched(std::string&& word)
	{
		using namespace std::string_literals;

		if (delimWasMatched())
		{
			Inserter{}(result, std::move(word));
		}
		else
		{
			throw std::runtime_error{ "Unmatched delimiter: "s + currentDelim };
		}
	}

	template <template <typename...> typename Container, typename Inserter>
	bool StringSplitter<Container, Inserter>::delimWasMatched()
	{
		if (currentDelim == ' ')
		{
			//white space is always matched, even by '\0'
			return true;
		}
		else
		{
			stream.unget();
			return stream.get() == currentDelim;
		}
	}
}



