#ifndef __STRING_H_INCLUDED__
#define __STRING_H_INCLUDED__

class String
{
public:
	String();                                 
	String(char symbol);                            
	String(const char* string);
	String(const String& source);
	String(String&& source);
	~String();

	String& operator=(String&& rhs);
	String& operator=(const String& rhs);

	operator const char*() const;

	size_t getLength() const;                          

	void append(const char* string);
	void append(char symbol);

	String& operator+=(const char* rhs);
	String& operator+=(char rhs);

private:
	char* actualString;

private:
	const char* getActualString() const;           
	void setActualString(const char* cString);  
	void setActualString(char symbol);
	void nullActualString();
	void destroyActualString();

	void moveParameterInThis(String& source);

	static char* cloneCString(const char* cString);
};

String operator+(const String& lhs, char rhs);
String operator+(char lhs, const String& rhs);
String operator+(const String& lhs, const String& rhs);

bool operator==(const String& lhs, const String& rhs); 
bool operator!=(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);

#endif // __STRING_H_INCLUDED__