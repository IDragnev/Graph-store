#include "UndirectedGraph.h"
#include "Graph Factory\Graph registrator\GraphRegistrator.h"
#include "Exceptions\Exceptions.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"

using namespace fmt::literals;

namespace IDragnev::GraphStore
{
	static GraphRegistrator<UndirectedGraph> registrator{ "undirected" };

	void UndirectedGraph::insertEdge(Vertex& start, Vertex& end, Edge::Weight weight)
	{
		assert(isOwnerOf(start));
		assert(isOwnerOf(end));

		if (!existsEdgeFromTo(start, end))
		{
			insertEdgeInBothDirections(start, end, weight);
		}
		else
		{
			throw DuplicateEdge{ start.getID(), end.getID() };
		}
	}

	void UndirectedGraph::insertEdgeInBothDirections(Vertex& start, Vertex& end, Edge::Weight weight)
	{
		Graph::insertEdgeFromToWithWeight(start, end, weight);
		try
		{
			Graph::insertEdgeFromToWithWeight(end, start, weight);
		}
		catch (NoMemoryAvailable&)
		{
			removeEdgeFromToNoThrow(start, end);
			throw;
		}
	}

	void UndirectedGraph::removeEdge(Vertex& start, Vertex& end)
	{
		assert(isOwnerOf(start));
		assert(isOwnerOf(end));

		Graph::removeEdgeFromTo(start, end);
		Graph::removeEdgeFromTo(end, start);
	}

	void UndirectedGraph::removeEdgesEndingIn(Vertex& vertex)
	{
		Graph::removeEdgesEndingIn(vertex);
	}

	String UndirectedGraph::getType() const
	{
		return "undirected";
	}

	auto UndirectedGraph::getConstIteratorToEdges() const -> EdgeConstIteratorPtr
	{
		return makeEdgeConstIterator();
	}

	std::size_t UndirectedGraph::getEdgesCount() const noexcept
	{
		return Graph::getEdgesCount() / 2;
	}
}