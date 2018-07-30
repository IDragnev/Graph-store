#include "DijkstraAlgorithm.h"


DijkstraAlgorithm::DijkstraAlgorithm() :
	Base("Dijkstra"),
	priorityQueue()
{
}


void DijkstraAlgorithm::findShortestPath(Graph& graph, Vertex& source, Vertex& goal)
{
	if (source != goal)
	{
		initializeSingleSource(graph, source);
		initializeState(graph, goal);
		findShortestPathToGoal();
		clearState();
	}
	else
	{
		findTrivialPathFromTo(source, goal);
	}
}


void DijkstraAlgorithm::initializeState(Graph& graph, const Vertex& goal)
{
	Base::initializeState(graph, goal);
	insertVerticesInPriorityQueue(graph);
}


void DijkstraAlgorithm::insertVerticesInPriorityQueue(Graph& graph)
{
	assert(priorityQueue.isEmpty());

	std::unique_ptr<ConstIterator<Vertex*>> iteratorPtr = graph.getIteratorToVertices();
	priorityQueue = MinPriorityQueue(*iteratorPtr, graph.getVerticesCount());
}


void DijkstraAlgorithm::findShortestPathToGoal()
{
	assert(hasVertexWithUndeterminedDistance());

	while (hasVertexWithUndeterminedDistance())
	{
		Vertex& vertex = closestToSourceFromUndetermined();

		if (isTheGoal(vertex))
		{
			break;
		}
		else
		{
			relaxEdgesLeaving(vertex);
		}
	}
}


void DijkstraAlgorithm::relaxEdgesLeaving(Vertex& vertex)
{
	std::unique_ptr<Iterator<Edge>> iterator = getIncidentEdgesOf(vertex);

	while (!iterator->isFinished())
	{
		relaxEdge(vertex, iterator->getCurrent());

		iterator->goToNext();
	}
}


void DijkstraAlgorithm::relaxEdge(Vertex& vertex, Edge& edge)
{
	Vertex& neighbour = edge.getIncidentVertex();
	Distance distanceBetween = edge.getWeight();
	Distance distanceTakingThisPath = vertex.getDistanceToSource() + distanceBetween;

	if (distanceTakingThisPath < neighbour.getDistanceToSource())
	{
		extendCurrentPath(vertex, neighbour, distanceTakingThisPath);
	}
}


void DijkstraAlgorithm::extendCurrentPath(Vertex& vertex, Vertex& neighbour, const Distance& distance)
{
	neighbour.setPredecessor(&vertex);
	updateDistanceOf(neighbour, distance);
}


void  DijkstraAlgorithm::updateDistanceOf(Vertex& vertex, const Distance& distance)
{
	priorityQueue.improveKey(vertex.getPriorityQueueHandle(), distance);
}


bool DijkstraAlgorithm::hasVertexWithUndeterminedDistance() const
{
	return !priorityQueue.isEmpty();
}


Vertex& DijkstraAlgorithm::closestToSourceFromUndetermined()
{
	Vertex* result = priorityQueue.extractOptimal();
	assert(result);

	return *result;
}


void DijkstraAlgorithm::clearState()
{
	priorityQueue.empty();
}


void DijkstraAlgorithm::initializeVertex(Vertex& vertex) const
{
	vertex.setPredecessor(nullptr);
	vertex.setDistanceToSource(Distance::getInfinity());
}


void DijkstraAlgorithm::initializeSource(Vertex& source) const
{
	source.setDistanceToSource(0);
}