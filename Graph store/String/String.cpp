#include "String.h"
#include <cstring>
#include <utility>


String::String() :
	content{ nullptr }
{ 
}


String::String(char symbol) :
	String{}
{
	content = new char[2] {symbol, '\0'};
}


String::String(const String& other) :
	String{ other.content }
{
}


String::String(const char* string) :
	String{}
{
	setContent(string);
}


String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		setContent(rhs.content);
	}

	return *this;
}


void String::setContent(const char* string)
{
	if (string)
	{
		char* copy = clone(string);
		delete[] content;
		content = copy;
	}
	else
	{
		delete[] content;
		content = nullptr;
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
	content{ source.content }
{
	source.content = nullptr;
}


String& String::operator=(String&& rhs)
{
	if (this != &rhs)
	{
		String temporary{ std::move(rhs) };
		std::swap(content, temporary.content);
	}

	return *this;
}


String::~String()
{
	delete[] content;
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

			//getContent() because this->content could be null
			strcpy_s(buffer, size, this->getContent());
			strcat_s(buffer, size, string);

			delete[] content;
			content = buffer;
		}
	}
}


void String::append(char symbol)
{
	char buffer[]{ symbol, '\0' };
	append(buffer);
}


const char* String::getContent() const
{
	return (content) ? content : "";
}


String::operator const char *() const
{
	return getContent();
}


size_t String::getLength() const
{
	return strlen(getContent());
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

