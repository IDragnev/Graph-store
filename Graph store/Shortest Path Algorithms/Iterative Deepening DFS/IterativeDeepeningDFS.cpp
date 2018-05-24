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

	for (unsigned depthBound = 0; depthBound <= maxDepth && !isPathFound; ++depthBound)
	{
		depthLimitedSearch(source, depthBound);
	}
}


void IterativeDeepeningDFS::depthLimitedSearch(Vertex& vertex, unsigned depth)
{
	vertex.markAsVisited();

	if (depth == 0 && isTheGoal(vertex))
	{
		isPathFound = true;
	}
	else if (depth > 0)
	{
		EdgeIteratorPtr edgeIterator(searchedGraph->getIteratorToIncidentEdgesOf(vertex));

		while (!edgeIterator->isFinished())
		{
			Edge& edge = edgeIterator->getCurrent();
			Vertex& neighbour = edge.getIncidentVertex();

			if (!neighbour.isVisited())
			{
				neighbour.setParentInAlgorithmTree(&vertex);
				neighbour.setDistanceToSource(vertex.getDistanceToSource() + Distance(1));

				depthLimitedSearch(neighbour, depth - 1);

				if (isPathFound)
				{
					break;
				}
			}
		}
	}

	vertex.markAsNotVisited();
}


void IterativeDeepeningDFS::initializeSingleSource(Graph& graph, Vertex& source)
{
	VertexIteratorPtr iterator(graph.getIteratorToVertices());

	while (!iterator->isFinished())
	{
		Vertex* vertex = iterator->getCurrent();

		vertex->setParentInAlgorithmTree(nullptr);
		vertex->setDistanceToSource(Distance::getInfinity());
		vertex->markAsNotVisited();

		iterator->goToNext();
	}

	source.setDistanceToSource(0);
}


bool IterativeDeepeningDFS::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


void IterativeDeepeningDFS::clearState()
{
	searchedGraph = nullptr;
	goal = nullptr;
	isPathFound = false;
}


void IterativeDeepeningDFS::initializeState(Graph& graph, const Vertex& goal)
{
	this->searchedGraph = &graph;
	this->goal = &goal;
	this->isPathFound = false;
}