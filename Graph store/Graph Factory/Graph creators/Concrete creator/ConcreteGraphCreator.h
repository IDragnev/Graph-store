#ifndef __CONCRETE_GRAPH_CREATOR__
#define __CONCRETE_GRAPH_CREATOR__

#include "../Base/GraphCreator.h"

template <typename CreatedGraph>
class ConcreteGraphCreator : public GraphCreator
{
public:
	ConcreteGraphCreator(const char* createdGraphType);
	virtual ~ConcreteGraphCreator() override = default;

	virtual std::unique_ptr<Graph> createEmptyGraph(const char* graphID) const override;

private:
	ConcreteGraphCreator(ConcreteGraphCreator&& source) = delete;
	ConcreteGraphCreator(const ConcreteGraphCreator& source) = delete;
	ConcreteGraphCreator& operator=(const ConcreteGraphCreator& rhs) = delete;
	ConcreteGraphCreator& operator=(ConcreteGraphCreator&& rhs) = delete;
};

#include "ConcreteGraphCreatorImpl.hpp"
#endif //__CONCRETE_GRAPH_CREATOR__