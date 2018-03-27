#include "String.h"
#include <utility>



String::String()
{
	nullValue();
}


String::String(const char* newValue)
{
	nullValue();
	setValue(newValue);
}


String::String(const String& other)
{
	nullValue();
	setValue(other.value);
}


void String::nullValue()
{
	value = nullptr;
}


String& String::operator=(const String& other)
{
	if (this != &other)
	{
		setValue(other.value);
	}

	return *this;
}



void String::setValue(const char* newValue)
{
	if (newValue == nullptr)
	{
		destroyValue();
		nullValue();
	}
	else
	{
		char* buffer = cloneCString(newValue);
		destroyValue();
		value = buffer;
	}
}



char* String::cloneCString(const char* cString)
{
	size_t size = strlen(cString) + 1;

	char* result = new char[size];
	strcpy_s(result, size, cString);

	return result;
}



void String::destroyValue()
{
	delete[] value;
}



String::String(char symbol)
{
	nullValue();
	value = cloneCString(symbol);
}


char* String::cloneCString(char symbol)
{
	char* result = new char[2];
	result[0] = symbol;
	result[1] = '\0';

	return result;
}



String::~String()
{
	destroyValue();
}



String::String(String&& source)
{
	moveSourceInThis(source);
}


String& String::operator=(String&& source)
{
	if (this != &source)
	{
		destroyValue();
		moveSourceInThis(source);
	}

	return *this;
}


void String::moveSourceInThis(String& source)
{
	value = source.value;
	source.nullValue();
}



void String::appendToValue(const char* newValue)
{
	if (newValue != nullptr)
	{
		size_t size = strlen(newValue);

		if (size > 0)
		{
			size += (this->len() + 1);
			char* buffer = new char[size];

			//getValue() because this->value could be null
			strcpy_s(buffer, size, this->getValue());
			strcat_s(buffer, size, newValue);

			destroyValue();
			value = buffer;
		}
	}
}



const char* String::getValue()const
{
	return isNotNull() ? value : "";
}


bool String::isNotNull()const
{
	return value != nullptr;
}


String::operator const char *()const
{
	return getValue();
}


size_t String::len()const
{
	return isNotNull() ? strlen(value) : 0;
}


bool operator==(const String& lhs, const String& rhs)
{
	return strcmp(lhs, rhs) == 0;
}

bool operator!=(const String& lhs, const String& rhs)
{
	return !(lhs == rhs);
}

bool operator>(const String& lhs, const String& rhs)
{
	return strcmp(lhs, rhs) > 0;
}


bool operator>=(const String& lhs, const String& rhs)
{
	return (lhs > rhs) || (lhs == rhs);
}


bool operator<(const String& lhs, const String& rhs)
{
	return !(lhs >= rhs);
}


bool operator<=(const String& lhs, const String& rhs)
{
	return !(lhs > rhs);
}



String& String::operator+=(const String& other)
{
	appendToValue(other);

	return *this;
}


String operator+(const String& lhs, const String& rhs)
{
	String result(lhs);

	result += rhs;

	return result;
}



String operator+(char lhs, const String& rhs)
{
	String result(lhs);

	result += rhs;

	return result;
}



String operator+(const char* lhs, const String& rhs)
{
	String result(lhs);

	result += rhs;

	return result;
}


