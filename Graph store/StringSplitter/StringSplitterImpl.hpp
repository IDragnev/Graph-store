
namespace IDragnev
{
	template <template <typename...> typename Container, typename Inserter>
	StringSplitter<Container, Inserter>::StringSplitter(std::initializer_list<char> delimiters) :
		delimiters{ delimiters }
	{
	}

	//all members except for delimiters act like local variables
	//and thus need not be copied
	template <template <typename...> typename Container, typename Inserter>
	StringSplitter<Container, Inserter>::StringSplitter(const StringSplitter& source) :
		delimiters{ source.delimiters }
	{
	}

	template <template <typename...> typename Container, typename Inserter>
	StringSplitter<Container, Inserter>::StringSplitter(StringSplitter&& source) :
		delimiters{ std::move(source.delimiters) }
	{
	}

	template <template <typename...> typename Container, typename Inserter>
	auto StringSplitter<Container, Inserter>::operator=(const StringSplitter& rhs) -> StringSplitter&
	{
		if (this != &rhs)
		{
			swapDelimsWith(rhs);
		}

		return *this;
	}

	template <template <typename...> typename Container, typename Inserter>
	auto StringSplitter<Container, Inserter>::operator=(StringSplitter&& rhs) -> StringSplitter&
	{
		if (this != &rhs)
		{
			swapDelimsWith(std::move(rhs));
		}

		return *this;
	}

	template <template <typename...> typename Container, typename Inserter>
	inline void StringSplitter<Container, Inserter>::swapDelimsWith(StringSplitter temp)
	{
		using std::swap;
		swap(delimiters, temp.delimiters);
	}

	template <template <typename...> typename Container, typename Inserter>
	Container<std::string> StringSplitter<Container, Inserter>::operator()(const std::string& str)
	{
		auto clear = makeScopedClear();
		init(str);
		split();

		return std::move(result);
	}

	template <template <typename...> typename Container, typename Inserter>
	auto StringSplitter<Container, Inserter>::makeScopedClear() noexcept
	{
		auto deleter = [this](auto)
		{ 
			stream.clear();
			result.clear();
			currentDelim = ' ';
		};
		
		using ScopedClear = std::unique_ptr<int, decltype(deleter)>;
		
		return ScopedClear{ nullptr, deleter };
	}

	template <template <typename...> typename Container, typename Inserter>
	inline void StringSplitter<Container, Inserter>::init(const std::string& str)
	{
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



