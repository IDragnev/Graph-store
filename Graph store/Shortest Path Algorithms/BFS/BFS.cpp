#include "BFS.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"
#include <memory>

static ShortestPathAlgorithmRegistrator<BFS> registrator{ "BFS" };


void BFS::findShortestPath(Graph& graph, Vertex& source, Vertex& goal)
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


void BFS::findShortestPathToGoalFrom(Vertex& source)
{
	assert(isFrontierEmpty());

	addToFrontier(source);

	while (!isFrontierEmpty())
	{
		Vertex& vertex = extractVertexFromTheFrontier();

		if (isTheGoal(vertex))
		{
			break;
		}
		else
		{
			expandFrontierFrom(vertex);
		}
	}
}


bool BFS::isFrontierEmpty() const
{
	return queue.isEmpty();
}


void BFS::addToFrontier(Vertex& vertex)
{
	assert(vertex.isVisited());

	queue.enqueue(&vertex);
}


Vertex& BFS::extractVertexFromTheFrontier()
{
	Vertex* vertex = queue.dequeue();
	assert(vertex);

	return *vertex;
}


void BFS::expandFrontierFrom(Vertex& vertex)
{
	std::unique_ptr<Iterator<Edge>> iterator = getEdgesLeaving(vertex);

	while (!iterator->isFinished())
	{
		Edge& edge = iterator->getCurrent();
		Vertex& neighbour = edge.getIncidentVertex();

		if (!neighbour.isVisited())
		{
			neighbour.markAsVisited();
			extendCurrentPathFromTo(vertex, neighbour);
			addToFrontier(neighbour);
		}

		iterator->goToNext();
	}
}


void BFS::extendCurrentPathFromTo(Vertex& from, Vertex& to)
{
	to.setPredecessor(&from);
	to.setDistanceToSource(from.getDistanceToSource() + Distance{ 1 });
}


void BFS::clearState()
{
	queue.empty();
}
