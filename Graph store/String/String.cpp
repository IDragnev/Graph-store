#include "String.h"
#include <cstring>


String::String() :
	actualString(nullptr)
{ 
}


String::String(const char* string) :
	String()
{
	setActualString(string);
}


String::String(const String& other) :
	String(other.actualString)
{
}


String::String(char symbol) :
	String()
{
	setActualString(symbol);
}


String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		setActualString(rhs.actualString);
	}

	return *this;
}


String& String::operator=(String&& source)
{
	if (this != &source)
	{
		destroyActualString();
		moveParameterInThis(source);
	}

	return *this;
}


String::String(String&& source)
{
	moveParameterInThis(source);
}


String::~String()
{
	destroyActualString();
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


void String::moveParameterInThis(String& source)
{
	actualString = source.actualString;
	source.nullActualString();
}


void String::setActualString(const char* cString)
{
	if (cString != nullptr)
	{
		char* copy = cloneCString(cString);
		destroyActualString();
		actualString = copy;
	}
	else
	{
		destroyActualString();
		nullActualString();
	}
}


void String::setActualString(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	setActualString(buffer);
}


char* String::cloneCString(const char* cString)
{
	size_t size = strlen(cString) + 1;

	char* result = new char[size];
	strcpy_s(result, size, cString);

	return result;
}


void String::append(const char* string)
{
	if (string != nullptr)
	{
		size_t size = strlen(string);

		if (size > 0)
		{
			size += (this->getLength() + 1);
			char* buffer = new char[size];

			//getActualString() because this->actualString could be null
			strcpy_s(buffer, size, this->getActualString());
			strcat_s(buffer, size, string);

			destroyActualString();
			actualString = buffer;
		}
	}
}


void String::append(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	append(buffer);
}


void String::destroyActualString()
{
	delete[] actualString;
}


void String::nullActualString()
{
	actualString = nullptr;
}  


const char* String::getActualString() const
{
	return (actualString != nullptr) ? actualString : "";
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


String operator+(const String& lhs, char rhs)
{
	String result(lhs);

	result += rhs;

	return result;
}

