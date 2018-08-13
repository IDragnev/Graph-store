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
	const char* getContent() const;           
	void setContent(const char* string);  
	
	static char* clone(const char* string);

private:
	char* content;
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