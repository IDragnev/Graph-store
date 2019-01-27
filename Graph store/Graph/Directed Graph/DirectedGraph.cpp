#include "DirectedGraph.h"
#include "..\..\General Exceptions\Exception.h"
#include "..\..\Graph Factory\Graph registrator\GraphRegistrator.h"

namespace GS = IDragnev::GraphStore;
static GS::GraphRegistrator<GS::DirectedGraph> registrator{ "directed" };

namespace IDragnev
{
	namespace GraphStore
	{
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
				throw Exception{ "Such edge already exists" };
			}
		}

		void DirectedGraph::removeEdge(Vertex& start, Vertex& end)
		{
			assert(isOwnerOf(start));
			assert(isOwnerOf(end));

			Graph::removeEdgeFromTo(start, end);
		}

		void DirectedGraph::removeEdgesEndingIn(Vertex& vertex)
		{
			auto iteratorPtr = getIteratorToVertices();

			forEach(*iteratorPtr, [&](Vertex& neighbour)
			{
				removeEdgeFromToNoThrow(neighbour, vertex);
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
	}
}