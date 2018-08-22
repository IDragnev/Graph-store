#include "DijkstraAlgorithm.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"
#include <memory>

static ShortestPathAlgorithmRegistrator<DijkstraAlgorithm> registrator{ "Dijkstra" };


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
	ShortestPathAlgorithm::initializeState(graph, goal);
	insertVerticesInPriorityQueue(graph);
}


void DijkstraAlgorithm::insertVerticesInPriorityQueue(Graph& graph)
{
	assert(priorityQueue.isEmpty());

	auto iteratorPtr = graph.getIteratorToVertices();
	priorityQueue = MinPriorityQueue{ *iteratorPtr, graph.getVerticesCount() };
}


void DijkstraAlgorithm::findShortestPathToGoal()
{
	assert(existsVertexWithUndeterminedDistance());

	while (existsVertexWithUndeterminedDistance())
	{
		auto& vertex = closestToSourceFromUndetermined();

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


bool DijkstraAlgorithm::existsVertexWithUndeterminedDistance() const
{
	return !priorityQueue.isEmpty();
}


Vertex& DijkstraAlgorithm::closestToSourceFromUndetermined()
{
	Vertex* result = priorityQueue.extractOptimal();
	assert(result);

	return *result;
}


void DijkstraAlgorithm::relaxEdgesLeaving(Vertex& vertex)
{
	auto iteratorPtr = getEdgesLeaving(vertex);
	forEach(*iteratorPtr, [&](Edge& edge) { relaxEdge(vertex, edge); });
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


void DijkstraAlgorithm::clearState()
{
	priorityQueue.empty();
}


void DijkstraAlgorithm::initializeVertex(Vertex& vertex) const
{
	vertex.setPredecessor(nullptr);
	vertex.setDistanceToSource(Distance::Infinity());
}


void DijkstraAlgorithm::initializeSource(Vertex& source) const
{
	source.setDistanceToSource(0);
}