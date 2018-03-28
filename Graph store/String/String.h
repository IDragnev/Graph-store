#ifndef __STRING_H_INCLUDED__
#define __STRING_H_INCLUDED__

class String
{
public:
	String();                                 
	String(char);                            
	String(const char*);
	String(const String&);
	String(String&&);
	~String();

	String& operator=(String&&);
	String& operator=(const String&);

	operator const char*()const;

	size_t getLength()const;                          

	void append(const char*);
	void append(char);

	String& operator+=(const char*);
	String& operator+=(char);

private:
	char* value;

private:
	const char* getValue()const;           
	void setValue(const char*);  
	void setValue(char);
	void nullValue();
	void destroyValue();

	void moveSourceInThis(String&);

	static char* cloneCString(const char*);
};


String operator+(char, const String&);
String operator+(const char*, const String&);
String operator+(const String&, const String&);

bool operator==(const String& lhs, const String& rhs); 
bool operator!=(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);

#endif // __STRING_H_INCLUDED__