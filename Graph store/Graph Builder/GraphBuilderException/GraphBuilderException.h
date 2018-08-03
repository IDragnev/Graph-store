#ifndef __GRAPH_BUILDER_EXC_H_INCLUDED__
#define __GRAPH_BUILDER_EXC_H_INCLUDED__

#include <stdexcept>

class GraphBuilderException : public std::runtime_error
{
public:
	GraphBuilderException(const char* message);
	GraphBuilderException(const std::string& message);
	virtual ~GraphBuilderException() override = default;
};

#endif //__GRAPH_BUILDER_EXC_H_INCLUDED__
