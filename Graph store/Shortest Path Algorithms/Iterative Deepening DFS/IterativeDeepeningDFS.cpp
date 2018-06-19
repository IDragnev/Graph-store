#include "IterativeDeepeningDFS.h"
#include "../../Graph/Base Graph/Graph.h"
#include <memory>
#include <assert.h>


IterativeDeepeningDFS IterativeDeepeningDFS::theOnlyInstance;


IterativeDeepeningDFS::IterativeDeepeningDFS() :
	Base("DFS-Shortest"),
	maxDepth(0)
{
	registerInstance();
}


void IterativeDeepeningDFS::findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal)
{
	if (source != goal)
	{
		initializeState(graph, goal);
		initializeSingleSource(graph, source);
		findShortestPathToGoalFrom(source);
	}
	else
	{
		findTrivialPathFromTo(source, goal);
	}
}


void IterativeDeepeningDFS::findShortestPathToGoalFrom(Vertex& source)
{
	assert(!isAShortestPathFound());

	for (unsigned depthBound = 0; !isAShortestPathFound() && depthBound <= maxDepth; ++depthBound)
	{
		startDepthLimitedSearchFromWithBound(source, depthBound);
	}
}


void IterativeDeepeningDFS::startDepthLimitedSearchFromWithBound(Vertex& vertex, unsigned depthBound)
{
	vertex.markAsVisited();

	if (depthBound == 0 && isTheGoal(vertex))
	{
		notifyAShortestPathWasFound();
	}
	else if (depthBound > 0)
	{
		proceedWithNeighboursOfWithBound(vertex, depthBound - 1);
	}

	vertex.markAsNotVisited();
}


void IterativeDeepeningDFS::proceedWithNeighboursOfWithBound(Vertex& vertex, unsigned depthBound)
{
	std::unique_ptr<Iterator<Edge>> edgeIterator = getIncidentEdgesOf(vertex);

	while (!edgeIterator->isFinished())
	{
		Edge& edge = edgeIterator->getCurrent();
		Vertex& neighbour = edge.getIncidentVertex();

		if (!neighbour.isVisited())
		{
			extendCurrentPathFromTo(vertex, neighbour);

			startDepthLimitedSearchFromWithBound(neighbour, depthBound);

			if (isAShortestPathFound())
			{
				return;
			}
		}

		edgeIterator->goToNext();
	}
}


void IterativeDeepeningDFS::extendCurrentPathFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	vertexTo.setPredecessor(&vertexFrom);
	vertexTo.setDistanceToSource(vertexFrom.getDistanceToSource() + Distance(1));
}


void IterativeDeepeningDFS::initializeState(Graph& graph, const Vertex& goal)
{
	Base::initializeState(graph, goal);

	//|V| - 1 because shortest paths are unique
	maxDepth = graph.getVerticesCount() - 1;
}