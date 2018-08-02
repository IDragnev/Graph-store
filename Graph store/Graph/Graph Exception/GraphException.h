#ifndef __GRAPH_EXCEPTION_H_INCLUDED__
#define __GRAPH_EXCEPTION_H_INCLUDED__

#include <stdexcept>

class GraphException : public std::runtime_error
{
public:
	GraphException(const char* message);
	GraphException(const std::string& message);
	virtual ~GraphException() override = default;
};

#endif //__GRAPH_EXCEPTION_H_INCLUDED__