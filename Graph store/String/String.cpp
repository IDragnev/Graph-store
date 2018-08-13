#include "String.h"
#include <cstring>
#include <utility>


String::String() :
	actualString{ nullptr }
{ 
}


String::String(char symbol) :
	String{}
{
	actualString = new char[2] {symbol, '\0'};
}


String::String(const String& other) :
	String{ other.actualString }
{
}


String::String(const char* string) :
	String{}
{
	setActualString(string);
}


String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		setActualString(rhs.actualString);
	}

	return *this;
}


void String::setActualString(const char* string)
{
	if (string)
	{
		char* copy = clone(string);
		delete[] actualString;
		actualString = copy;
	}
	else
	{
		delete[] actualString;
		actualString = nullptr;
	}
}


char* String::clone(const char* string)
{
	size_t size = strlen(string) + 1;
	char* result = new char[size];
	strcpy_s(result, size, string);

	return result;
}


String::String(String&& source) :
	actualString{ source.actualString }
{
	source.actualString = nullptr;
}


String& String::operator=(String&& rhs)
{
	if (this != &rhs)
	{
		String temporary{ std::move(rhs) };
		std::swap(actualString, temporary.actualString);
	}

	return *this;
}


String::~String()
{
	delete[] actualString;
}


String& String::operator+=(const char* string)
{
	append(string);

	return *this;
}


String& String::operator+=(char symbol)
{
	append(symbol);

	return *this;
}


void String::append(const char* string)
{
	if (string)
	{
		size_t size = strlen(string);

		if (size > 0)
		{
			size += (this->getLength() + 1);
			char* buffer = new char[size];

			//getActualString() because this->actualString could be null
			strcpy_s(buffer, size, this->getActualString());
			strcat_s(buffer, size, string);

			delete[] actualString;
			actualString = buffer;
		}
	}
}


void String::append(char symbol)
{
	char buffer[]{ symbol, '\0' };
	append(buffer);
}


const char* String::getActualString() const
{
	return (actualString) ? actualString : "";
}


String::operator const char *() const
{
	return getActualString();
}


size_t String::getLength() const
{
	return strlen(getActualString());
}


bool operator==(const String& lhs, const String& rhs)
{
	return strcmp(lhs, rhs) == 0;
}


bool operator!=(const String& lhs, const String& rhs)
{
	return !(lhs == rhs);
}


bool operator<(const String& lhs, const String& rhs)
{
	return strcmp(lhs, rhs) < 0;
}


bool operator>(const String& lhs, const String& rhs)
{
	return rhs < lhs;
}


bool operator<=(const String& lhs, const String& rhs)
{
	return !(lhs > rhs);
}


bool operator>=(const String& lhs, const String& rhs)
{
	return !(lhs < rhs);
}


String operator+(const String& lhs, const String& rhs)
{
	String result{ lhs };
	result += rhs;

	return result;
}


String operator+(char lhs, const String& rhs)
{
	String result{ lhs };
	result += rhs;

	return result;
}


String operator+(const String& lhs, char rhs)
{
	String result{ lhs };
	result += rhs;

	return result;
}

