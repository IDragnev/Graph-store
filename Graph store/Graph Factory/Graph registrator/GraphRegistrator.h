#ifndef __GRAPH_REGISTRATOR_H_INCLUDED__
#define __GRAPH_REGISTRATOR_H_INCLUDED__

#include "../Graph creators/Concrete creator/ConcreteGraphCreator.h"

template <typename GraphType>
class GraphRegistrator
{
public:
	explicit GraphRegistrator(const String& createdGraphType);
	~GraphRegistrator() = default;

private:
	GraphRegistrator(GraphRegistrator&&) = delete;
	GraphRegistrator(const GraphRegistrator&) = delete;
	GraphRegistrator& operator=(GraphRegistrator&&) = delete;
	GraphRegistrator& operator=(const GraphRegistrator&) = delete;

private:
	const ConcreteGraphCreator<GraphType> graphCreator;
};

#include "GraphRegistratorImpl.hpp"
#endif //__GRAPH_REGISTRATOR_H_INCLUDED__