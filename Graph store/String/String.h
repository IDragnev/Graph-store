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

	String& operator+=(const String&);          

private:
	char* value;

private:
	const char* getValue()const;           
	void setValue(const char*);  
	void appendToValue(const char*);

	bool isNotNull()const;
	void nullValue();
	void destroyValue();

	void moveSourceInThis(String&);

private:
	static char* cloneCString(const char*);
	static char* cloneCString(char);
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