#ifndef __STRING_H_INCLUDED__
#define __STRING_H_INCLUDED__

class String
{
public:
	String();                                 
	String(char);                            
	String(const char* value);
	String(const String&);
	String(String&&);
	~String();

	String& operator=(String&&);
	String& operator=(const String&);

	operator const char*()const;

	size_t len()const;                          

	String& operator+=(const char*);          
	String& operator+=(char);

private:
	char* str;

private:
	const char* getValue()const;           
	void setValue(const char*);  
	void append(const char*);
};


String operator+(const String&, char);
String operator+(char, const String&);
String operator+(const String&, const String&);
String operator+(const char*, const String&);
String operator+(const String&, const char*);

bool operator==(const String& s1, const String& s2); 
bool operator!=(const String& s1, const String& s2);
bool operator>(const String& s1, const String& s2);  
bool operator>=(const String& s1, const String& s2);
bool operator<(const String& s1, const String& s2); 
bool operator<=(const String& s1, const String& s2);

#endif // __STRING_H_INCLUDED__