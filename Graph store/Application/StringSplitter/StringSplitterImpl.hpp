
template <template <class...> class Container>
Container<std::string> StringSplitter<Container>::split(const std::string& string, char delimiter)
{
	init(string, delimiter);
	split();
	
	return std::move(result);
}


template <template <class...> class Container>
void StringSplitter<Container>::init(const std::string& string, char delimiter)
{
	stream.clear();
	stream.str(string);
	delim = delimiter;
}


template <template <class...> class Container>
void StringSplitter<Container>::split()
{
	std::string word;

	while (std::getline(stream, word, delim))
	{
		result.push_back(std::move(word));
		ignoreWhileDelim();
	}
}


template <template <class...> class Container>
void StringSplitter<Container>::ignoreWhileDelim()
{
	while (stream.peek() == delim)
	{
		stream.ignore();
	}
}
