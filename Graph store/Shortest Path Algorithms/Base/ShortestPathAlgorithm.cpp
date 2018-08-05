#include "ShortestPathAlgorithm.h"
#include <assert.h>


ShortestPathAlgorithm::ShortestPathAlgorithm(const char* ID) :
	id(ID),
	searchedGraph(nullptr),
	goal(nullptr)
{
	assert(id != String(""));
}


void ShortestPathAlgorithm::findTrivialPathFromTo(Vertex& source, Vertex& goal)
{
	assert(source == goal);

	goal.setPredecessor(nullptr);
	goal.setDistanceToSource(0);
}


void ShortestPathAlgorithm::initializeSingleSource(Graph& graph, Vertex& source) const
{
	std::unique_ptr<ConstIterator<Vertex*>> iterator = graph.getIteratorToVertices();

	while (!iterator->isFinished())
	{
		Vertex* vertex = iterator->getCurrent();
		initializeVertex(*vertex);

		iterator->goToNext();
	}

	initializeSource(source);
}


void ShortestPathAlgorithm::initializeVertex(Vertex& vertex) const
{
	vertex.setPredecessor(nullptr);
	vertex.setDistanceToSource(Distance::Infinity());
	vertex.markAsNotVisited();
}


void ShortestPathAlgorithm::initializeSource(Vertex& source) const
{
	source.markAsVisited();
	source.setDistanceToSource(0);
}


bool ShortestPathAlgorithm::isTheGoal(const Vertex& vertex) const
{
	return vertex == *goal;
}


std::unique_ptr<Iterator<Edge>> ShortestPathAlgorithm::getIncidentEdgesOf(Vertex& vertex) const
{
	return searchedGraph->getIteratorToIncidentEdgesOf(vertex);
}


void ShortestPathAlgorithm::initializeState(Graph& graph, const Vertex& Goal)
{
	searchedGraph = &graph;
	goal = &Goal;
}


const String& ShortestPathAlgorithm::getID() const
{
	return id;
}