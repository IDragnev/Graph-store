#include "IterativeDeepeningDFS.h"
#include "../../Graph/Base Graph/Graph.h"
#include <memory>
#include <assert.h>


IterativeDeepeningDFS IterativeDeepeningDFS::theOnlyInstance;


IterativeDeepeningDFS::IterativeDeepeningDFS() :
	ShortestPathAlgorithm("DFS-Shortest"),
	searchedGraph(nullptr),
	goal(nullptr),
	isPathFound(false)
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
		clearState();
	}
	else
	{
		findTrivialPathFromTo(source, goal);
	}
}


void IterativeDeepeningDFS::findShortestPathToGoalFrom(Vertex& source)
{
	assert(!isAShortestPathFound() && searchedGraph);

	//maxDepth = |V| - 1 since shortest paths are unique
	unsigned maxDepth = searchedGraph->getVerticesCount() - 1;

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
	std::unique_ptr<Iterator<Edge>> edgeIterator(searchedGraph->getIteratorToIncidentEdgesOf(vertex));

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


bool IterativeDeepeningDFS::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


bool IterativeDeepeningDFS::isAShortestPathFound() const
{
	return isPathFound;
}


void IterativeDeepeningDFS::notifyAShortestPathWasFound()
{
	assert(!isPathFound);

	isPathFound = true;
}


void IterativeDeepeningDFS::initializeState(Graph& graph, const Vertex& Goal)
{
	assert(!searchedGraph && !goal);

	searchedGraph = &graph;
	goal = &Goal;
	isPathFound = false;
}


void IterativeDeepeningDFS::clearState()
{
	searchedGraph = nullptr;
	goal = nullptr;
	isPathFound = false;
}