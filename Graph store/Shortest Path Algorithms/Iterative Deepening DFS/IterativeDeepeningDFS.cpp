#include "IterativeDeepeningDFS.h"
#include "../../Graph/Base Graph/Graph.h"
#include <memory>
#include <assert.h>

typedef std::unique_ptr<Iterator<Edge>> EdgeIteratorPtr;
typedef std::unique_ptr<ConstIterator<Vertex*>> VertexIteratorPtr;

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
	initializeState(graph, goal);
	initializeSingleSource(graph, source);
	findShortestPathToGoal(source);
	clearState();
}


void IterativeDeepeningDFS::findShortestPathToGoal(Vertex& source)
{
	assert(!isPathFound && searchedGraph);

	//maxDepth = |V| - 1 since shortest paths are unique
	unsigned maxDepth = searchedGraph->getVerticesCount() - 1;

	for (unsigned depthBound = 0; !isPathFound && depthBound <= maxDepth; ++depthBound)
	{
		startDepthLimitedSearchFromWithBound(source, depthBound);
	}
}


void IterativeDeepeningDFS::startDepthLimitedSearchFromWithBound(Vertex& vertex, unsigned depthBound)
{
	vertex.markAsVisited();

	if (depthBound == 0 && isTheGoal(vertex))
	{
		isPathFound = true;
	}
	else if (depthBound > 0)
	{
		proceedWithNeighboursOfWithBound(vertex, depthBound - 1);
	}

	vertex.markAsNotVisited();
}


void IterativeDeepeningDFS::proceedWithNeighboursOfWithBound(Vertex& vertex, unsigned depthBound)
{
	EdgeIteratorPtr edgeIterator(searchedGraph->getIteratorToIncidentEdgesOf(vertex));

	while (!edgeIterator->isFinished())
	{
		Edge& edge = edgeIterator->getCurrent();
		Vertex& neighbour = edge.getIncidentVertex();

		if (!neighbour.isVisited())
		{
			neighbour.setPredecessor(&vertex);
			neighbour.setDistanceToSource(vertex.getDistanceToSource() + Distance(1));

			startDepthLimitedSearchFromWithBound(neighbour, depthBound);

			if (isPathFound)
			{
				return;
			}
		}
	}
}


bool IterativeDeepeningDFS::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


void IterativeDeepeningDFS::initializeSingleSource(Graph& graph, Vertex& source)
{
	VertexIteratorPtr iterator(graph.getIteratorToVertices());

	while (!iterator->isFinished())
	{
		Vertex* vertex = iterator->getCurrent();

		vertex->setPredecessor(nullptr);
		vertex->setDistanceToSource(Distance::getInfinity());
		vertex->markAsNotVisited();

		iterator->goToNext();
	}

	source.setDistanceToSource(0);
}


void IterativeDeepeningDFS::initializeState(Graph& graph, const Vertex& goal)
{
	this->searchedGraph = &graph;
	this->goal = &goal;
	this->isPathFound = false;
}


void IterativeDeepeningDFS::clearState()
{
	searchedGraph = nullptr;
	goal = nullptr;
	isPathFound = false;
}