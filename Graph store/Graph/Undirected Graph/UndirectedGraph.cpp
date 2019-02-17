#include "UndirectedGraph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"
#include "../../General Exceptions/NoMemoryAvailable.h"
#include "..\..\..\Third party\fmt-5.3.0\include\fmt\format.h"

namespace GS = IDragnev::GraphStore;
using namespace fmt::literals;

static GS::GraphRegistrator<GS::UndirectedGraph> registrator{ "undirected" };

namespace IDragnev
{
	namespace GraphStore
	{
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
				throw Exception{ fmt::format("And edge from {u} to {v} already exists!", "u"_a = start.ID(), "v"_a = end.ID()) };
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
	}
}