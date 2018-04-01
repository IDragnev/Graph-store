#include "String.h"
#include <utility>



String::String() :
	actualString(nullptr)
{
	;
}


String::String(const char* string) :
	actualString(nullptr)
{
	setActualString(string);
}


String::String(const String& other) :
	actualString(nullptr)
{
	setActualString(other.actualString);
}



String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		setActualString(rhs.actualString);
	}

	return *this;
}



void String::setActualString(const char* cString)
{
	if (cString != nullptr)
	{
		char* buffer = cloneCString(cString);
		destroyActualString();
		actualString = buffer;
	}
	else
	{
		destroyActualString();
		nullActualString();
	}
}



char* String::cloneCString(const char* cString)
{
	size_t size = strlen(cString) + 1;

	char* result = new char[size];
	strcpy_s(result, size, cString);

	return result;
}



void String::destroyActualString()
{
	delete[] actualString;
}


void String::nullActualString()
{
	actualString = nullptr;
}  

 

String::String(char symbol) :
	actualString(nullptr)
{
	setActualString(symbol);
}



void String::setActualString(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	setActualString(buffer);
}



String::~String()
{
	destroyActualString();
}



String::String(String&& source)
{
	moveParameterInThis(source);
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


void String::moveParameterInThis(String& source)
{
	actualString = source.actualString;
	source.nullActualString();
}



String& String::operator+=(const char* string)
{
	append(string);

	return *this;
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



String& String::operator+=(char symbol)
{
	append(symbol);

	return *this;
}



void String::append(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	append(buffer);
}



const char* String::getActualString()const
{
	return (actualString != nullptr) ? actualString : "";
}


String::operator const char *()const
{
	return getActualString();
}


size_t String::getLength()const
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


bool operator<=(const String& lhs, const String& rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}


bool operator>(const String& lhs, const String& rhs)
{
	return !(lhs <= rhs);
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

