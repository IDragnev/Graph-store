#include "ShortestPathAlgorithm.h"
#include <assert.h>


ShortestPathAlgorithm::ShortestPathAlgorithm(const char* ID) :
	id{ ID },
	searchedGraph{ nullptr },
	goal{ nullptr }
{
	assert(id != String{ "" });
}


bool ShortestPathAlgorithm::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


ShortestPathAlgorithm::EdgeConstIteratorPtr
ShortestPathAlgorithm::getEdgesLeaving(const Vertex& v) const
{
	return searchedGraph->getConstIteratorToEdgesLeaving(v);
}


void ShortestPathAlgorithm::initBase(Graph& graph, const Vertex& goal)
{
	this->searchedGraph = &graph;
	this->goal = &goal;
}


const String& ShortestPathAlgorithm::getID() const
{
	return id;
}