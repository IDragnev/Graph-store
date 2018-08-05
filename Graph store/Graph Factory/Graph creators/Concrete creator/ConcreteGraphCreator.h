#ifndef __CONCRETE_GRAPH_CREATOR__
#define __CONCRETE_GRAPH_CREATOR__

#include "../Base/GraphCreator.h"

template <typename CreatedGraph>
class ConcreteGraphCreator : public GraphCreator
{
public:
	ConcreteGraphCreator(const char* createdGraphType);
	virtual ~ConcreteGraphCreator() override = default;

	virtual GraphPtr createEmptyGraph(const char* graphID) const override;

private:
	ConcreteGraphCreator(ConcreteGraphCreator&&) = delete;
	ConcreteGraphCreator(const ConcreteGraphCreator&) = delete;
	ConcreteGraphCreator& operator=(const ConcreteGraphCreator&) = delete;
	ConcreteGraphCreator& operator=(ConcreteGraphCreator&&) = delete;
};

#include "ConcreteGraphCreatorImpl.hpp"
#endif //__CONCRETE_GRAPH_CREATOR__