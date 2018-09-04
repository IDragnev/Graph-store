#include "BFSShortest.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"
#include <memory>

static ShortestPathAlgorithmRegistrator<BFSShortest> registrator{ "BFS" };


ShortestPathAlgorithm::Path 
BFSShortest::findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal)
{
	wrap(graph, source);
	auto result = findShortestPathToGoalFrom(source);
	clean();

	return std::move(result);
}


void BFSShortest::wrap(const Graph& graph, const Vertex& source)
{
	auto iteratorPtr = graph.getIteratorToVertices();

	forEach(*iteratorPtr, [&](const Vertex* v)
	{
		wrappers.try_emplace(v->getID(), v);
	});

	auto& sourceWrapper = wrappers[source.getID()];
	sourceWrapper.distanceToSource = 0;
	sourceWrapper.predecessor = nullptr;
	sourceWrapper.isVisited = true;
}


ShortestPathAlgorithm::Path
BFSShortest::findShortestPathToGoalFrom(const Vertex& source)
{
	assert(isFrontierEmpty());

	addToFrontier(source);

	while (!isFrontierEmpty())
	{
		auto& vertex = extractVertexFromTheFrontier();

		if (isTheGoal(vertex))
		{
			return Path{ wrappers[vertex.getID()] };
		}
		else
		{
			expandFrontierFrom(vertex);
		}
	}

	return Path{};
}


bool BFSShortest::isFrontierEmpty() const
{
	return queue.isEmpty();
}


void BFSShortest::addToFrontier(const Vertex& vertex)
{
	queue.enqueue(&vertex);
}


const Graph::Vertex& BFSShortest::extractVertexFromTheFrontier()
{
	auto* vertex = queue.dequeue();
	assert(vertex);

	return *vertex;
}


void BFSShortest::expandFrontierFrom(const Vertex& vertex)
{
	auto iteratorPtr = getEdgesLeaving(vertex);

	forEach(*iteratorPtr, [&](const Edge& edge) 
	{
		auto& neighbour = edge.getIncidentVertex();
		auto& neighbourWrapper = wrappers[neighbour.getID()];

		if (!neighbourWrapper.isVisited)
		{
			neighbourWrapper.isVisited = true;
			extendCurrentPathFromTo(wrappers[vertex.getID()], neighbourWrapper);
			addToFrontier(neighbour);
		}
	});
}


void BFSShortest::extendCurrentPathFromTo(const BFSVertex& from, BFSVertex& to)
{
	to.predecessor = &from;
	to.distanceToSource = from.distanceToSource + Distance{ 1 };
}


void BFSShortest::clean()
{
	queue.empty();
	wrappers.clear();
}
