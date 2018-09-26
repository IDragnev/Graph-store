

template <template <typename...> typename Container>
StringSplitter<Container>::StringSplitter(std::initializer_list<char> delimiters) :
	stream{},
	result{},
	delimiters{ delimiters }
{
}


template <template <typename...> typename Container>
Container<std::string> StringSplitter<Container>::split(const std::string& string)
{
	init(string);
	split();
	
	return std::move(result);
}


template <template <typename...> typename Container>
inline void StringSplitter<Container>::init(const std::string& string)
{
	stream.clear();
	stream.str(string);
}


template <template <typename...> typename Container>
void StringSplitter<Container>::split()
{
	do
	{
		skipWhiteSpaces();
		auto delim = chooseDelimiter();
		advanceIfNotWhiteSpace(delim);
		extractWord(delim);
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
char StringSplitter<Container>::chooseDelimiter()
{ 
	auto currentSymbol = stream.peek();

	for (const auto& delim : delimiters)
	{
		if (delim == currentSymbol)
		{
			return delim;
		}
	}

	return ' ';
}


template <template <typename...> typename Container>
inline void StringSplitter<Container>::advanceIfNotWhiteSpace(char delimiter)
{
	if (delimiter != ' ')
	{
		stream.ignore();
	}
}


template <template <typename...> typename Container>
void StringSplitter<Container>::extractWord(char delim)
{
	auto word = std::string{};
	std::getline(stream, word, delim);

	if (!stream.fail())
	{
		insertIfMatched(std::move(word), delim);
	}
}


template <template <typename...> typename Container>
void StringSplitter<Container>::insertIfMatched(std::string&& word, char delim)
{
	if (wasMatched(delim))
	{
		result.insert(std::move(word));
	}
	else
	{
		throw Exception{ "Unmatched delimiter: " + std::string{ delim } };
	}
}


template <template <typename...> typename Container>
bool StringSplitter<Container>::wasMatched(char delimiter)
{
	if (delimiter == ' ')
	{
		//white space is always matched, even by '\0'
		return true;
	}
	else
	{
		stream.unget();
		return stream.get() == delimiter;
	}
}




