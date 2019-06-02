#include "String.h"
#include <cstring>
#include <algorithm>

namespace IDragnev
{
	String::String(char c) :
		content{ new char[2]{ c, '\0' } }
	{
	}

	String::String(const String& other) :
		String{ other.content }
	{
	}

	String::String(const char* string)
	{
		setContent(string);
	}

	String& String::operator=(const String& rhs)
	{
		if (this != &rhs)
		{
			setContent(rhs.content);
		}

		return *this;
	}

	void String::setContent(const char* string)
	{
		resetContentWith(string != nullptr ? clone(string) : nullptr);
	}

	void String::resetContentWith(char* string)
	{
		delete[] content;
		content = string;
	}

	char* String::clone(const char* string)
	{
		auto size = std::strlen(string) + 1u;
		auto result = new char[size];
		std::copy_n(string, size, result);

		return result;
	}

	String::String(String&& source) noexcept :
		content{ source.content }
	{
		source.content = nullptr;
	}

	String& String::operator=(String&& rhs) noexcept
	{
		if (this != &rhs)
		{
			auto temp = std::move(rhs);
			std::swap(content, temp.content);
		}

		return *this;
	}

	String::~String()
	{
		delete[] content;
	}

	String& String::operator+=(const String& string)
	{
		append(string.getContent());

		return *this;
	}

	String& String::operator+=(const char* string)
	{
		append(string);

		return *this;
	}

	String& String::operator+=(char c)
	{
		char buffer[]{ c, '\0' };
		append(buffer);

		return *this;
	}

	void String::append(const char* string)
	{
		if (string == nullptr) return;

		if (auto sourceLength = std::strlen(string);
			sourceLength > 0u)
		{
			auto currentLength = getLength();
			auto buffer = new char[sourceLength + currentLength + 1u];

			std::copy_n(this->content, currentLength, buffer);
			std::copy_n(string, sourceLength + 1u, buffer + currentLength);

			resetContentWith(buffer);
		}
	}

	const char* String::getContent() const noexcept
	{
		return (content != nullptr) ? content : "";
	}

	String::operator const char *() const noexcept
	{
		return getContent();
	}

	size_t String::getLength() const noexcept
	{
		return std::strlen(getContent());
	}

	bool operator==(const String& lhs, const String& rhs) noexcept
	{
		return std::strcmp(lhs.getContent(), rhs.getContent()) == 0u;
	}

	bool operator!=(const String& lhs, const String& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	bool operator<(const String& lhs, const String& rhs) noexcept
	{
		return std::strcmp(lhs.getContent(), rhs.getContent()) < 0u;
	}

	bool operator>(const String& lhs, const String& rhs) noexcept
	{
		return rhs < lhs;
	}

	bool operator<=(const String& lhs, const String& rhs) noexcept
	{
		return !(lhs > rhs);
	}

	bool operator>=(const String& lhs, const String& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	String operator+(const String& lhs, const String& rhs)
	{
		auto result = lhs;
		result += rhs;

		return result;
	}

	String operator+(char lhs, const String& rhs)
	{
		auto result = String{ lhs };
		result += rhs;

		return result;
	}

	String operator+(const String& lhs, char rhs)
	{
		auto result = lhs;
		result += rhs;

		return result;
	}
}
