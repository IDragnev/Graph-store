#include "String.h"
#include <utility>

//---------------------------------------------------------------
//
//GET AND SET
//


const char* String::getValue()const
{
	return (theRealString != nullptr) ? theRealString : "";
}


String::operator const char *()const
{
	return getValue();
}



void String::setValue(const char* value)
{
	if (!value)
	{
		delete[] theRealString;
		theRealString = nullptr;
	}
	else
	{
		size_t size = strlen(value) + 1;
		char* buffer = new char[size];
		strcpy_s(buffer, size, value);

		delete[] theRealString;
		theRealString = buffer;
	}
}



size_t String::len()const
{
	return (theRealString != nullptr) ? strlen(theRealString) : 0;
}



void String::appendToValue(const char* value)
{
	if (value)
	{
		size_t size = strlen(value);

		//if not empty
		if (size > 0)
		{
			size += (this->len() + 1);
			char* buffer = new char[size];

			//getValue() because theRealString could be null
			strcpy_s(buffer, size, getValue());
			strcat_s(buffer, size, value);

			delete[] theRealString;
			theRealString = buffer;
		}
	}
}


//----------------------------------------------------------------------------------------------
//
//CTORS
//



String::String()
	:
	theRealString(nullptr)
{
	;
}


String::String(const char* value)
	:
	theRealString(nullptr)
{
	setValue(value);
}


String::String(const String& other)
	:
	theRealString(nullptr)
{
	setValue(other.theRealString);
}



String::String(char c)
	:
	theRealString(nullptr)
{
	theRealString = new char[2];
	theRealString[0] = c;
	theRealString[1] = '\0';
}



String::~String()
{
	delete[] theRealString;
}



//
//move constructor
//
String::String(String&& source)
	:
	theRealString(source.theRealString)
{
	source.theRealString = nullptr;
}




//
// combined operator= taking by value and using the copy-and-swap idiom
//
// \ if rhs is an rvalue, 'other' will be initialized with move c-tor
//
// \ if rhs is an lvalue, 'other' will be copy-constructed which is fine
//   because we would have to reconstruct other's theRealString anyway
//
// \ if rhs is const char*, 'other' will be constructed with String(const char*)
//   which is fine again, because we would have to reconstruct this->theRealString anyway
//
// After constructing 'other' with rhs, data is swapped and 'other' destroys old data.
//
//String& String::operator=(String other)
//{
//	std::swap(this->theRealString, other.theRealString);
//
//	return *this;
//}
//



//
// (!) setValue frees old memory (if any)
//
String& String::operator=(const String& other)
{
	if (this != &other)
	{
		setValue(other.theRealString);
	}

	return *this;
}



//
//move assignment, can be called for rvalues only 
//(including const char* which will move-construct an rvalue object)
//
String& String::operator=(String&& other)
{
	if (this != &other)
	{
		delete[] theRealString;

		theRealString = other.theRealString;

		other.theRealString = nullptr;
	}

	return *this;
}



//----------------------------------------------------------------------
//
//OPERATORS
//


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


