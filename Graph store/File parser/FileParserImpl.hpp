
namespace IDragnev::GraphStore
{
	template <typename ArithmeticType>
	ArithmeticType FileParser::parseSigned()
	{
		static_assert(std::is_signed_v<ArithmeticType>,
			          "template <typename T> T FileParser::parseSigned() requires T to be a signed arithmetic type");
		
		auto prepare = [] {};
		auto message = "Invalid signed number format";

		return parseArithmeticType<ArithmeticType>(message, prepare);
	}

	template <typename ArithmeticType>
	ArithmeticType FileParser::parseUnsigned()
	{
		static_assert(std::is_unsigned_v<ArithmeticType>,
			          "template <typename T> T FileParser::parseUnsigned() requires T to be an unsigned arithmetic type");
		
		auto prepare = [this] { invalidateStreamIfSigned(); };
		auto message = "Invalid unsigned number format";
		
		return parseArithmeticType<ArithmeticType>(message, prepare);
	}

	template <typename ArithmeticType, typename Callable>
	ArithmeticType FileParser::parseArithmeticType(const char* messageOnError, Callable prepare)
	{
		validateState();

		prepare();
		auto result = parseArithmeticType<ArithmeticType>();
		throwIfParseFailed(messageOnError);

		return result;
	}

	template <typename ArithmeticType>
	ArithmeticType FileParser::parseArithmeticType()
	{
		ArithmeticType result = 0;
		stream >> result;

		return result;
	}
}