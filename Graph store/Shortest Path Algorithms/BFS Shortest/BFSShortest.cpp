#include "BFSShortest.h"
#include "../../Graph/Base Graph/Graph.h"
#include <assert.h>
#include <memory>

typedef std::unique_ptr<Iterator<Edge>> EdgeIteratorPtr;
typedef std::unique_ptr<ConstIterator<Vertex*>> VertexIteratorPtr;

BFSShortest BFSShortest::theOnlyInstance;


BFSShortest::BFSShortest() :
	ShortestPathAlgorithm("BFS-Shortest"),
	searchedGraph(nullptr),
	goal(nullptr),
	verticesQueue(),
	isPathFound(false)
{
	registerInstance();
}


void BFSShortest::findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal)
{
	if (source != goal)
	{
		initializeState(graph, goal);
		initializeSingleSource(graph, source);
		findShortestPathFrom(source);
		clearState();
	}
	else
	{
		buildTrivialPathFromTo(source, goal);
	}
}


void BFSShortest::findShortestPathFrom(Vertex& source)
{
	assert(!isAShortestPathFound());
	assert(isFrontierEmpty());

	addToFrontier(source);

	while (!(isAShortestPathFound() || isFrontierEmpty()))
	{
		Vertex& vertex = getVertexFromTheFrontier();

		exploreNeighboursOf(vertex);
	}
}


void BFSShortest::exploreNeighboursOf(Vertex& vertex)
{
	EdgeIteratorPtr edgeIterator(searchedGraph->getIteratorToIncidentEdgesOf(vertex));

	while (!edgeIterator->isFinished())
	{
		Edge& currentEdge = edgeIterator->getCurrent();
		Vertex& neighbour = currentEdge.getIncidentVertex();

		if (!isOnFrontier(neighbour))
		{
			processNeighbourDiscoveredFrom(neighbour, vertex);
			addToFrontier(neighbour);
		}

		edgeIterator->goToNext();
	}
}


void BFSShortest::processNeighbourDiscoveredFrom(Vertex& neighbour, Vertex& vertexFrom)
{
	extendCurrentPathFromTo(vertexFrom, neighbour);

	if (isTheGoal(neighbour))
	{
		notifyAShortestPathWasFound();
	}
}


void BFSShortest::extendCurrentPathFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	vertexTo.setPredecessor(&vertexFrom);
	vertexTo.setDistanceToSource(vertexFrom.getDistanceToSource() + Distance(1));
}