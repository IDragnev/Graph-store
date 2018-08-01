#ifndef __FILE_PARSER_EXCEPTION_H_INCLUDED__
#define __FILE_PARSER_EXCEPTION_H_INCLUDED__

#include <stdexcept>

class FileParserException : public std::runtime_error
{
public:
	FileParserException(const char* message);
	virtual ~FileParserException() override = default;
};

#endif //__FILE_PARSER_EXCEPTION_H_INCLUDED__