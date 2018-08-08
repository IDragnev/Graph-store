#include "BFSShortest.h"
#include "../../Graph/Base Graph/Graph.h"
#include <assert.h>
#include <memory>


BFSShortest::BFSShortest() :
	Base("BFS-Shortest"),
	verticesQueue()
{
}


void BFSShortest::findShortestPath(Graph& graph, Vertex& source, Vertex& goal)
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


void BFSShortest::initializeState(Graph& graph, const Vertex& goal)
{
	assert(verticesQueue.isEmpty());

	Base::initializeState(graph, goal);
}


void BFSShortest::findShortestPathToGoalFrom(Vertex& source)
{
	assert(isFrontierEmpty());

	addToFrontier(source);

	while (!isFrontierEmpty())
	{
		Vertex& vertex = extractVertexFromTheFrontier();

		if (isTheGoal(vertex))
		{
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
	return verticesQueue.isEmpty();
}


void BFSShortest::addToFrontier(Vertex& vertex)
{
	assert(vertex.isVisited());

	verticesQueue.enqueue(&vertex);
}


Vertex& BFSShortest::extractVertexFromTheFrontier()
{
	Vertex* vertex = verticesQueue.dequeue();
	assert(vertex);

	return *vertex;
}


void BFSShortest::expandFrontierFrom(Vertex& vertex)
{
	std::unique_ptr<Iterator<Edge>> edgeIterator = getIncidentEdgesOf(vertex);

	while (!edgeIterator->isFinished())
	{
		Edge& currentEdge = edgeIterator->getCurrent();
		Vertex& neighbour = currentEdge.getIncidentVertex();

		if (!neighbour.isVisited())
		{
			neighbour.markAsVisited();
			extendCurrentPathFromTo(vertex, neighbour);
			addToFrontier(neighbour);
		}

		edgeIterator->goToNext();
	}
}


void BFSShortest::extendCurrentPathFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	vertexTo.setPredecessor(&vertexFrom);
	vertexTo.setDistanceToSource(vertexFrom.getDistanceToSource() + Distance(1));
}


void BFSShortest::clearState()
{
	verticesQueue.empty();
}
