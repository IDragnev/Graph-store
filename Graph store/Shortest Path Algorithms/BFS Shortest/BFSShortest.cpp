#include "BFSShortest.h"
#include "../../Graph/Base Graph/Graph.h"
#include <assert.h>


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
