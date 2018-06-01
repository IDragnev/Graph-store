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
		findShortestPathToGoalFrom(source);
		clearState();
	}
	else
	{
		buildTrivialPathFromTo(source, goal);
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
	EdgeIteratorPtr edgeIterator(searchedGraph->getIteratorToIncidentEdgesOf(vertex));

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


bool BFSShortest::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


bool BFSShortest::isAShortestPathFound() const
{
	return isPathFound;
}


void BFSShortest::notifyAShortestPathWasFound()
{
	assert(!isPathFound);

	isPathFound = true;
}


void BFSShortest::initializeSingleSource(Graph& graph, Vertex& source)
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

	source.markAsVisited();
	source.setDistanceToSource(0);
}


void BFSShortest::initializeState(Graph& graph, const Vertex& Goal)
{
	assert(!searchedGraph && !goal);
	assert(verticesQueue.isEmpty());

	searchedGraph = &graph;
	goal = &Goal;
	isPathFound = false;
}


void BFSShortest::clearState()
{
	searchedGraph = nullptr;
	goal = nullptr;
	isPathFound = false;
	verticesQueue.empty();
}
