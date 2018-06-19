#include "BFSShortest.h"
#include "../../Graph/Base Graph/Graph.h"
#include <assert.h>
#include <memory>


BFSShortest BFSShortest::theOnlyInstance;


BFSShortest::BFSShortest() :
	Base("BFS-Shortest"),
	verticesQueue()
{
	registerInstance();
}


void BFSShortest::findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal)
{
	if (source != goal)
	{
		initializeState(graph, goal);
		initializeSingleSource(graph, source);
		findShortestPathToGoalFrom(source);
		clearState();
	}
	else
	{
		findTrivialPathFromTo(source, goal);
	}
}


void BFSShortest::findShortestPathToGoalFrom(Vertex& source)
{
	assert(!isAShortestPathFound());
	assert(isFrontierEmpty());

	addToFrontier(source);

	while (!(isAShortestPathFound() || isFrontierEmpty()))
	{
		Vertex& vertex = extractVertexFromTheFrontier();

		exploreNeighboursOf(vertex);
	}
}


void BFSShortest::exploreNeighboursOf(Vertex& vertex)
{
	std::unique_ptr<Iterator<Edge>> edgeIterator(searchedGraph->getIteratorToIncidentEdgesOf(vertex));

	while (!edgeIterator->isFinished())
	{
		Edge& currentEdge = edgeIterator->getCurrent();
		Vertex& neighbour = currentEdge.getIncidentVertex();

		if (!neighbour.isVisited())
		{
			neighbour.markAsVisited();
			visitNeighbourDiscoveredFrom(neighbour, vertex);
			addToFrontier(neighbour);
		}

		edgeIterator->goToNext();
	}
}


void BFSShortest::visitNeighbourDiscoveredFrom(Vertex& neighbour, Vertex& vertexFrom)
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


void BFSShortest::addToFrontier(Vertex& vertex)
{
	assert(vertex.isVisited());

	verticesQueue.enqueue(&vertex);
}


Vertex& BFSShortest::extractVertexFromTheFrontier()
{
	Vertex* vertex = verticesQueue.dequeue();

	return *vertex;
}


bool BFSShortest::isFrontierEmpty() const
{
	return verticesQueue.isEmpty();
}


void BFSShortest::initializeState(Graph& graph, const Vertex& goal)
{
	assert(verticesQueue.isEmpty());

	Base::initializeState(graph, goal);
}


void BFSShortest::clearState()
{
	verticesQueue.empty();
}
