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
