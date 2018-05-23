#include "IterativeDeepeningDFS.h"
#include "../../Graph/Base Graph/Graph.h"
#include <memory>
#include <assert.h>

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
		findPathWithRestrictedDepth(&source, depthBound);
	}
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


bool IterativeDeepeningDFS::isTheGoal(const Vertex* const vertex) const
{
	return vertex == goal;
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