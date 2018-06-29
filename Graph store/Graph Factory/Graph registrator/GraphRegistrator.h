#ifndef __GRAPH_REGISTRATOR_H_INCLUDED__
#define __GRAPH_REGISTRATOR_H_INCLUDED__

#include "../Graph creators/Concrete creator/ConcreteGraphCreator.h"

template <typename GraphType>
class GraphRegistrator
{
public:
	GraphRegistrator(const char* createdGraphType);

private:
	ConcreteGraphCreator<GraphType> creator;
};

#endif //__GRAPH_REGISTRATOR_H_INCLUDED__