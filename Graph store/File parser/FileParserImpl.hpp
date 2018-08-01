#include "../Traits/IsInteger.h"
#include <assert.h>


template <typename Integer>
Integer FileParser::parseInteger()
{
	static_assert(IsInteger<Integer>::value, "template <class T> T FileParser::parseInteger() requires T to be an integer type");
	assert(hasOpenedFile());
	assert(!hasReachedEnd());
	assert(!file.fail());

	Integer result = 0;
	file >> result;

	throwIfParseFailed("Invalid integer format");

	return result;
}
