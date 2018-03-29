#include "String.h"
#include <utility>



String::String()
	:
	value(nullptr)
{
	;
}


String::String(const char* newValue)
	:
	value(nullptr)
{
	setValue(newValue);
}


String::String(const String& other)
	:
	value(nullptr)
{
	setValue(other.value);
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
	if (newValue != nullptr)
	{
		char* buffer = cloneCString(newValue);
		destroyValue();
		value = buffer;
	}
	else
	{
		destroyValue();
		nullValue();
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


void String::nullValue()
{
	value = nullptr;
}  

 

String::String(char symbol)
	:
	value(nullptr)
{
	setValue(symbol);
}



void String::setValue(char symbol)
{
	char buffer[2] = "";
	buffer[0] = symbol;

	setValue(buffer);
}



String::~String()
{
	destroyValue();
}



String::String(String&& source)
{
	moveParameterInThis(source);
}


String& String::operator=(String&& source)
{
	if (this != &source)
	{
		destroyValue();
		moveParameterInThis(source);
	}

	return *this;
}


void String::moveParameterInThis(String& source)
{
	value = source.value;
	source.nullValue();
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

			//getValue() because this->value could be null
			strcpy_s(buffer, size, this->getValue());
			strcat_s(buffer, size, string);

			destroyValue();
			value = buffer;
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



const char* String::getValue()const
{
	return (value != nullptr) ? value : "";
}


String::operator const char *()const
{
	return getValue();
}


size_t String::getLength()const
{
	return strlen(getValue());
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


