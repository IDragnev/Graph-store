#include "IterativeDeepeningDFS.h"

IterativeDeepeningDFS IterativeDeepeningDFS::theOnlyInstance;


IterativeDeepeningDFS::IterativeDeepeningDFS() :
	ShortestPathAlgorithm("DFS-Shortest"),
	searchedGraph(nullptr),
	goal(nullptr),
	isPathFound(false)
{
}