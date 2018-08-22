

template <template <class...> class Container>
StringSplitter<Container>::StringSplitter(std::initializer_list<char> delimiters) :
	stream{},
	result{},
	delimiters{ delimiters }
{
}


template <template <class...> class Container>
Container<std::string> StringSplitter<Container>::split(const std::string& string)
{
	init(string);
	split();
	
	return std::move(result);
}


template <template <class...> class Container>
inline void StringSplitter<Container>::init(const std::string& string)
{
	stream.clear();
	stream.str(string);
}


template <template <class...> class Container>
void StringSplitter<Container>::split()
{
	do
	{
		skipWhiteSpaces();
		char delim = chooseDelimiter();
		advanceIfNotWhiteSpace(delim);
		extractWord(delim);
	} while (stream.good());

}


template <template <class...> class Container>
void StringSplitter<Container>::skipWhiteSpaces()
{
	while (stream.peek() == ' ')
	{
		stream.ignore();
	}
}


template <template <class...> class Container>
char StringSplitter<Container>::chooseDelimiter()
{ 
	char current = stream.peek();
	auto iterator = std::find_if(delimiters.getBeginConstIterator(), delimiters.getEndConstIterator(), 
		[&](char delimiter)
	{
		return delimiter == current;
	});

	return (iterator) ? *iterator : ' ';
}


template <template <class...> class Container>
inline void StringSplitter<Container>::advanceIfNotWhiteSpace(char delimiter)
{
	if (delimiter != ' ')
	{
		stream.ignore();
	}
}


template <template <class...> class Container>
void StringSplitter<Container>::extractWord(char delimiter)
{
	std::string word;
	std::getline(stream, word, delimiter);

	if (!stream.fail() && wasMatched(delimiter))
	{
		result.insert(std::move(word));
	}
}


template <template <class...> class Container>
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




