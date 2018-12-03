#ifndef __STRING_H_INCLUDED__
#define __STRING_H_INCLUDED__

namespace IDragnev
{
	class String
	{
	public:
		String() = default;
		String(char symbol);
		String(const char* string);
		String(const String& source);
		String(String&& source) noexcept;
		~String();

		String& operator=(String&& rhs) noexcept;
		String& operator=(const String& rhs);

		operator const char*() const noexcept;

		size_t getLength() const noexcept;

		void append(const char* string);
		void append(char symbol);

		String& operator+=(const char* rhs);
		String& operator+=(char rhs);

	private:
		const char* getContent() const noexcept;
		void setContent(const char* string);

		static char* clone(const char* string);

	private:
		char* content = nullptr;
	};

	String operator+(const String& lhs, char rhs);
	String operator+(char lhs, const String& rhs);
	String operator+(const String& lhs, const String& rhs);

	bool operator==(const String& lhs, const String& rhs) noexcept;
	bool operator!=(const String& lhs, const String& rhs) noexcept;
	bool operator<(const String& lhs, const String& rhs) noexcept;
	bool operator<=(const String& lhs, const String& rhs) noexcept;
	bool operator>(const String& lhs, const String& rhs) noexcept;
	bool operator>=(const String& lhs, const String& rhs) noexcept;
}

#endif // __STRING_H_INCLUDED__