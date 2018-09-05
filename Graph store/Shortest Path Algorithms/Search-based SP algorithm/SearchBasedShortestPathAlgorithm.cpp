#include "SearchBasedShortestPathAlgorithm.h"


void SearchBasedShortestPathAlgorithm::decorate(const Graph& graph, const Vertex& source)
{
	auto iteratorPtr = graph.getIteratorToVertices();

	forEach(*iteratorPtr, [&](const Vertex* v)
	{
		decorators.try_emplace(v->getID(), v);
	});

	initSourceDecorator(decoratorOf(source));
}


void SearchBasedShortestPathAlgorithm::initSourceDecorator(MarkableVertex& source)
{
	source.distance = 0;
	source.predecessor = nullptr;
	source.isVisited = true;
}


SearchBasedShortestPathAlgorithm::MarkableVertex&
SearchBasedShortestPathAlgorithm::decoratorOf(const Vertex& v)
{
	return const_cast<MarkableVertex&>(
		static_cast<const SearchBasedShortestPathAlgorithm&>(*this).decoratorOf(v));
}


const SearchBasedShortestPathAlgorithm::MarkableVertex&
SearchBasedShortestPathAlgorithm::decoratorOf(const Vertex& v) const
{
	auto iterator = decorators.find(v.getID());
	assert(iterator != decorators.cend());

	auto pair = *iterator;
	return std::get<1>(pair);
}


void SearchBasedShortestPathAlgorithm::cleanDecoratedState()
{
	decorators.clear();
}