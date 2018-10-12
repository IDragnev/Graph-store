#include "DijkstraAlgorithm.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"
#include <memory>

static ShortestPathAlgorithmRegistrator<DijkstraAlgorithm> registrator{ "Dijkstra" };


ShortestPathAlgorithm::Path
DijkstraAlgorithm::findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal)
{
	decorate(graph, source);
	findShortestPath();
	clearState();

	return std::move(result);
}


void DijkstraAlgorithm::decorate(const Graph& graph, const Vertex& source)
{
	decorate(graph);
	buildDecoratorsMap();
	initSourceDecorator(decoratorOf(source));
	buildPriorityQueue();
}


void DijkstraAlgorithm::decorate(const Graph& graph)
{
	assert(decorators.isEmpty());

	auto iteratorPtr = graph.getIteratorToVertices();

	forEach(*iteratorPtr, [&](const Vertex* v) 
	{ 
		decorators.insert(PriorityVertex{ v });
	});
}


void DijkstraAlgorithm::buildDecoratorsMap()
{
	assert(map.isEmpty());
	using std::begin;
	using std::end;
	map = DecoratorsMap{ begin(decorators), end(decorators) };
}


void DijkstraAlgorithm::initSourceDecorator(PriorityVertex& source)
{
	assert(queue.isEmpty());
	source.distance = 0;
	source.predecessor = nullptr;
}


void DijkstraAlgorithm::buildPriorityQueue()
{
	assert(queue.isEmpty());
	using std::begin;
	using std::end;
	auto pointers = decoratorsPointers();
	queue = MinPriorityQueue{ begin(pointers), end(pointers) };
}


DijkstraAlgorithm::PointersCollection
DijkstraAlgorithm::decoratorsPointers()
{
	auto result = PointersCollection{};

	for (auto&& d : decorators)
	{
		result.insert(&d);
	}

	return result;
}


void DijkstraAlgorithm::findShortestPath()
{
	assert(existsVertexWithUndeterminedDistance());

	while (existsVertexWithUndeterminedDistance())
	{
		auto& vertex = closestToSourceFromUndetermined();

		if (isTheGoal(vertex))
		{
			result = Path{ vertex };
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
	return !queue.isEmpty();
}


const DijkstraAlgorithm::PriorityVertex&
DijkstraAlgorithm::closestToSourceFromUndetermined()
{
	auto* result = queue.extractOptimal();
	assert(result);
	return *result;
}


void DijkstraAlgorithm::relaxEdgesLeaving(const PriorityVertex& vertex)
{
	auto iteratorPtr = getEdgesLeaving(vertex);

	forEach(*iteratorPtr, [&](const Edge& edge)
	{ 
		relaxEdge(vertex, edge);
	});
}


void DijkstraAlgorithm::relaxEdge(const PriorityVertex& vertex, const Edge& edge)
{
	auto& neighbour = decoratorOf(edge.getIncidentVertex());
	auto distanceBetween = Distance{ edge.getWeight() };
	auto distanceTakingThisPath = vertex.distance + distanceBetween;

	if (distanceTakingThisPath < neighbour.distance)
	{
		extendCurrentPath(vertex, neighbour, distanceTakingThisPath);
	}
}


void DijkstraAlgorithm::extendCurrentPath(const PriorityVertex& from, PriorityVertex& to, const Distance& d)
{
	to.predecessor = &from;
	updateDistanceOf(to, d);
}


void  DijkstraAlgorithm::updateDistanceOf(PriorityVertex& v, const Distance& d)
{
	queue.improveKey(v.handle, d);
}


void DijkstraAlgorithm::clearState()
{
	map.empty();
	queue.empty();
	decorators.empty();
}


DijkstraAlgorithm::PriorityVertex&
DijkstraAlgorithm::decoratorOf(const Vertex& v)
{
	return const_cast<PriorityVertex&>( static_cast<const DijkstraAlgorithm&>(*this).decoratorOf(v) );
}


const DijkstraAlgorithm::PriorityVertex&
DijkstraAlgorithm::decoratorOf(const Vertex& v) const
{
	auto* result = map.search(v.getID());
	assert(result);
	return *result;
}