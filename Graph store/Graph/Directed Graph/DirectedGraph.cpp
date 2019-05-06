#include "DirectedGraph.h"
#include "Exceptions\Exceptions.h"
#include "Graph Factory\Graph registrator\GraphRegistrator.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "Graph\Base Graph\GraphUtilities.h"

using namespace fmt::literals;

namespace IDragnev::GraphStore
{
	static GraphRegistrator<DirectedGraph> registrator{ "directed" };

	bool DirectedGraph::DirectedEdgeConstIterator::wasCurrentEdgeIterated() const
	{
		return !(this->operator bool());
	}

	void DirectedGraph::insertEdge(Vertex& start, Vertex& end, Edge::Weight weight)
	{
		assert(isOwnerOf(start));
		assert(isOwnerOf(end));

		if (!existsEdgeFromTo(start, end))
		{
			Graph::insertEdgeFromToWithWeight(start, end, weight);
		}
		else
		{
			throw DuplicateEdge{ start.getID(), end.getID() };
		}
	}

	void DirectedGraph::removeEdge(Vertex& start, Vertex& end)
	{
		assert(isOwnerOf(start));
		assert(isOwnerOf(end));

		Graph::removeEdgeFromTo(start, end);
	}

	void DirectedGraph::removeEdgesEndingIn(Vertex& v)
	{
		forEachVertex(*this, [&to = v](Vertex& from)
		{
			removeEdgeFromToNoThrow(from, to);
		});
	}

	String DirectedGraph::getType() const
	{
		return "directed";
	}

	auto DirectedGraph::getConstIteratorToEdges() const -> EdgeConstIteratorPtr
	{
		return makeEdgeConstIterator<DirectedEdgeConstIterator>();
	}

	std::size_t DirectedGraph::getEdgesCount() const noexcept
	{
		return Graph::getEdgesCount();
	}
}