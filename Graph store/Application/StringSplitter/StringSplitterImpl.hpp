
namespace IDragnev
{
	namespace SplitterDetail
	{
		template <typename Container>
		auto insert(Container& c, std::string&& str) -> std::void_t<decltype(c.push_back(str))>
		{
			c.push_back(std::move(str));
		}

		template <typename Container, typename = void>
		auto insert(Container& c, std::string&& str) -> std::void_t<decltype(c.insert(str))>
		{
			c.insert(std::move(str));
		}
	}

	template <template <typename...> typename Container>
	StringSplitter<Container>::StringSplitter(std::initializer_list<char> delimiters) :
		delimiters{ delimiters }
	{
	}

	template <template <typename...> typename Container>
	Container<std::string> StringSplitter<Container>::split(const std::string& str)
	{
		init(str);
		split();

		return std::move(result);
	}

	template <template <typename...> typename Container>
	inline void StringSplitter<Container>::init(const std::string& str)
	{
		stream.clear();
		stream.str(str);
	}

	template <template <typename...> typename Container>
	void StringSplitter<Container>::split()
	{
		do
		{
			skipWhiteSpaces();
			chooseDelimiter();
			advanceIfDelimIsNotWhiteSpace();
			extractWord();
		} while (stream.good());
	}

	template <template <typename...> typename Container>
	void StringSplitter<Container>::skipWhiteSpaces()
	{
		while (stream.peek() == ' ')
		{
			stream.ignore();
		}
	}

	template <template <typename...> typename Container>
	void StringSplitter<Container>::chooseDelimiter()
	{
		auto currentSymbol = stream.peek();
		currentDelim = ' ';

		for (const auto& d : delimiters)
		{
			if (d == currentSymbol)
			{
				currentDelim = d;
				return;
			}
		}
	}

	template <template <typename...> typename Container>
	inline void StringSplitter<Container>::advanceIfDelimIsNotWhiteSpace()
	{
		if (currentDelim != ' ')
		{
			stream.ignore();
		}
	}

	template <template <typename...> typename Container>
	void StringSplitter<Container>::extractWord()
	{
		auto word = std::string{};
		std::getline(stream, word, currentDelim);

		if (!stream.fail())
		{
			insertIfDelimWasMatched(std::move(word));
		}
	}

	template <template <typename...> typename Container>
	void StringSplitter<Container>::insertIfDelimWasMatched(std::string&& word)
	{
		using SplitterDetail::insert;

		if (wasDelimMatched())
		{
			insert(result, std::move(word));
		}
		else
		{
			throw std::runtime_error{ "Unmatched delimiter: " + std::string{ currentDelim } };
		}
	}

	template <template <typename...> typename Container>
	bool StringSplitter<Container>::wasDelimMatched()
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



