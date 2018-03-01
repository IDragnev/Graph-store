#include "String.h"
#include <utility>

//---------------------------------------------------------------
//
//GET AND SET
//


//
//if str is nullptr return an empty string
//else return str
//
const char* String::getValue()const
{
	return (str) ? str : "";
}


String::operator const char *()const
{
	return getValue();
}



//
//if the sent value is nullptr 
//free the pointer and set str as nullptr
//
//else reconstruct str with the sent value
//
void String::setValue(const char* value)
{
	if (!value)
	{
		delete[] str;
		str = nullptr;
	}
	else
	{
		size_t size = strlen(value) + 1;
		char* buffer = new char[size];
		strcpy_s(buffer, size, value);

		delete[] str;
		str = buffer;
	}
}



//
//get the length of the string
//
// 0 if nullptr, strlen else
//
size_t String::len()const
{
	return (str) ? strlen(str) : 0;
}


//
//if the sent value is null, do nothing
//else reconstruct str with a concatenated string
//
void String::append(const char* value)
{
	if (value)
	{
		size_t size = strlen(value);

		//if not empty
		if (size > 0)
		{
			size += (this->len() + 1);
			char* buffer = new char[size];

			//copy this->str (could be null) and 'append' value
			strcpy_s(buffer, size, getValue());
			strcat_s(buffer, size, value);

			delete[] str;
			str = buffer;
		}
	}
}

//----------------------------------------------------------------------------------------------
//
//CTORS
//

//
//holds a null value by default
//
String::String()
	:
	str(nullptr)
{
	;
}



//
//constructs str with the sent value
//
String::String(const char* value)
	:
	str(nullptr)
{
	setValue(value);
}


//
//reconstructs str with other's str
//
String::String(const String& other)
	:
	str(nullptr)
{
	setValue(other.str);
}



//
//makes a new string with two 'boxes'
//one of which is the symbol
//and the other is '\0'
//
String::String(char c)
	:
	str(nullptr)
{
	str = new char[2];
	str[0] = c;
	str[1] = '\0';
}


//
//free str
//
String::~String()
{
	delete[] str;
}


//
//move constructor
//
//transfers source's string to this object
//
String::String(String&& source)
	:
	str(source.str)
{
	source.str = nullptr;
}




//
// combined operator= taking by value and using the copy-and-swap idiom
//
// \ if rhs is an rvalue, 'other' will be initialized with move c-tor
//
// \ if rhs is an lvalue, 'other' will be copy-constructed which is fine
//   because we would have to reconstruct other's str anyway
//
// \ if rhs is const char*, 'other' will be constructed with String(const char*)
//   which is fine again, because we would have to reconstruct this->str anyway
//
// After constructing 'other' with rhs, data is swapped and 'other' destroys old data.
//
//String& String::operator=(String other)
//{
//	std::swap(this->str, other.str);
//
//	return *this;
//}
//



//
//copy assignment, can be called for lvalues only
//
// setValue frees old memory (if any)
//
String& String::operator=(const String& other)
{
	if (this != &other)
	{
		setValue(other.str);
	}

	return *this;
}



//
//move assignment, can be called for rvalues only 
//(including const char* which will move-construct a rvalue object)
//
//moves the argument's resource to the current object, freeing the old resource
//
String& String::operator=(String&& other)
{
	if (this != &other)
	{
		delete[] str;

		str = other.str;

		other.str = nullptr;
	}

	return *this;
}



//----------------------------------------------------------------------
//
//OPERATORS
//


bool operator==(const String& s1, const String& s2)
{
	return strcmp(s1, s2) == 0;
}

bool operator!=(const String& s1, const String& s2)
{
	return !(s1 == s2);
}

bool operator>(const String& s1, const String& s2)
{
	return strcmp(s1, s2) > 0;
}


bool operator>=(const String& s1, const String& s2)
{
	return (s1 > s2) || (s1 == s2);
}


bool operator<(const String& s1, const String& s2)
{
	return !(s1 >= s2);
}


bool operator<=(const String& s1, const String& s2)
{
	return !(s1 > s2);
}





String& String::operator+=(const char* value)
{
	append(value);

	return *this;
}


String operator+(const String& string1, const String& string2)
{
	String temp(string1);

	temp += string2;

	return temp;
}


String operator+(const String& string, const char* str)
{
	String temp(string);

	temp += str;

	return temp;
}



String& String::operator+=(char c)
{
	static char temp[2];

	temp[0] = c;
	temp[1] = '\0';
	
	append(temp);

	return *this;
}



String operator+(const String& string, char c)
{
	String temp(string);

	temp += c;

	return temp;
}


String operator+(char c, const String& string)
{
	String str(c);

	str += string;

	return str;
}



String operator+(const char* str, const String& string)
{
	String temp(str);

	temp += string;                                         

	return temp;
}


