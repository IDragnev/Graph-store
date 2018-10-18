#include <type_traits>
#include <assert.h>

namespace IDragnev
{
	namespace GraphStore
	{
		template <typename ArithmeticType>
		ArithmeticType FileParser::parseSigned()
		{
			static_assert(std::is_signed<ArithmeticType>::value,
						  "template <typename T> T FileParser::parseSigned() requires T to be a signed arithmetic type");
			validateState();

			auto result = parseArithmeticType<ArithmeticType>();
			throwIfParseFailed("Invalid signed number format");

			return result;
		}

		template <typename ArithmeticType>
		ArithmeticType FileParser::parseUnsigned()
		{
			static_assert(std::is_unsigned<ArithmeticType>::value,
						  "template <typename T> T FileParser::parseUnsigned() requires T to be an unsigned arithmetic type");
			validateState();

			invalidateStreamIfSigned();
			auto result = parseArithmeticType<ArithmeticType>();
			throwIfParseFailed("Invalid unsigned number format");

			return result;
		}

		template <typename ArithmeticType>
		ArithmeticType FileParser::parseArithmeticType()
		{
			auto result = ArithmeticType{ 0 };
			stream >> result;

			return result;
		}
	}
}