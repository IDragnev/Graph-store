#include "BFSShortest.h"
#include "..\..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"

static ShortestPathAlgorithmRegistrator<BFSShortest> registrator{ "BFS" };


ShortestPathAlgorithm::Path 
BFSShortest::findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal)
{
	decorate(graph, source);
	findShortestPath(source, goal);
	clean();

	return std::move(result);
}


void BFSShortest::findShortestPath(const Vertex& source, const Vertex& goal)
{
	assert(isFrontierEmpty());
	addToFrontier(decoratorOf(source));

	while (!isFrontierEmpty())
	{
		auto& vertex = extractVertexFromFrontier();

		if (isTheGoal(vertex))
		{
			result = Path{ vertex };
			break;
		}
		else
		{
			expandFrontierFrom(vertex);
		}
	}
}


bool BFSShortest::isFrontierEmpty() const
{
	return queue.isEmpty();
}


void BFSShortest::addToFrontier(const MarkableVertex& v)
{
	assert(v.isVisited);
	queue.enqueue(&v);
}


const BFSShortest::MarkableVertex&
BFSShortest::extractVertexFromFrontier()
{
	auto* vertex = queue.dequeue();
	assert(vertex);

	return *vertex;
}


void BFSShortest::expandFrontierFrom(const MarkableVertex& vertex)
{
	auto iteratorPtr = getEdgesLeaving(vertex);

	forEach(*iteratorPtr, [&](const Edge& edge) 
	{
		auto& neighbour = decoratorOf(edge.getIncidentVertex());

		if (!neighbour.isVisited)
		{
			neighbour.isVisited = true;
			extendCurrentPathFromTo(vertex, neighbour);
			addToFrontier(neighbour);
		}
	});
}


void BFSShortest::extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to)
{
	to.predecessor = &from;
	to.distance = from.distance + Distance{ 1 };
}


void BFSShortest::clean()
{
	queue.empty();
	cleanDecoratedState();
}


void BFSShortest::initSourceDecorator(MarkableVertex& source)
{
	Base::initSourceDecorator(source);
}