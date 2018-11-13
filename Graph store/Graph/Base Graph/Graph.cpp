#include "Graph.h"
#include "..\..\General Exceptions\NoMemoryAvailable.h"
#include "..\..\ForwardIterator wrapper\ForwardIteratorWrapper.h"
#include <assert.h>
#include <algorithm>

using namespace IDragnev::Iterators;

namespace IDragnev
{
	namespace GraphStore
	{
		Graph::IncidentEdge::IncidentEdge(Vertex& incidentVertex, Weight weight) :
			incidentVertex{ incidentVertex },
			weight{ weight }
		{
		}

		Graph::Vertex& Graph::IncidentEdge::getIncidentVertex()
		{
			return const_cast<Vertex&>( static_cast<const IncidentEdge&>(*this).getIncidentVertex() );
		}

		const Graph::Vertex& Graph::IncidentEdge::getIncidentVertex() const
		{
			return incidentVertex;
		}

		auto Graph::IncidentEdge::getWeight() const -> Weight
		{
			return weight;
		}

		Graph::Vertex::Vertex(const String& ID, std::size_t index, EdgeListIterator edges) :
			index{ index },
			edges{ edges }
		{
			setID(ID);
		}

		void Graph::Vertex::setID(const String& ID)
		{
			if (ID != String{ "" })
			{
				id = ID;
			}
			else
			{
				throw Exception{ "A Vertex ID must be a valid string" };
			}
		}

		bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs)
		{
			//vertices are equal if and only if 
			//they are the same object
			return &lhs == &rhs;
		}

		bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs)
		{
			return !(lhs == rhs);
		}

		Graph::Edge::Edge(const Vertex& start, const IncidentEdge& edge) :
			startVertex{ start },
			incidentEdge{ edge }
		{
		}

		auto Graph::Edge::start() const -> const Vertex&
		{
			return startVertex;
		}

		auto Graph::Edge::end() const -> const Vertex&
		{
			return incidentEdge.get().getIncidentVertex();
		}

		auto Graph::Edge::weight() const -> Weight
		{
			return incidentEdge.get().getWeight();
		}	

		Graph::Graph(const String& ID) :
			verticesSearchTable{ FEWEST_VERTICES_EXPECTED }
		{
			//vectices.reserve(FEWEST_VERTICES_EXPECTED);
			setID(ID);
		}

		void Graph::setID(const String& ID)
		{
			if (ID != String{ "" })
			{
				id = ID;
			}
			else
			{
				throw Exception{ "A Graph ID must be a valid string" };
			}
		}

		void Graph::insertVertexWithID(const String& ID)
		{
			if (!hasVertexWithID(ID))
			{
				tryToInsertVertexWithID(ID);
			}
			else
			{
				throw Exception{ "A vertex with such ID already exists" };
			}
		}

		bool Graph::hasVertexWithID(const String& ID) const
		{
			return verticesSearchTable.search(ID) != nullptr;
		}

		void Graph::tryToInsertVertexWithID(const String& ID)
		{
			try
			{
				vertices.push_back(makeVertex(ID));
				insertInSearchTable(vertices.back());
			}
			catch (std::bad_alloc&)
			{
				throw NoMemoryAvailable{};
			}
		}

		auto Graph::makeVertex(const String& ID) const -> Vertex
		{
			//make a new EdgeList...
			return Vertex{ ID, vertices.size(), EdgeListsCollection{}.begin() };
		}

		void Graph::insertInSearchTable(Vertex& vertex)
		{
			try
			{
				verticesSearchTable.insert(vertex);
			}
			catch (std::bad_alloc&)
			{
				removeFromVertices(vertex);
				throw;
			}
		}

		void Graph::removeFromVertices(Vertex& vertex)
		{
			assert(isOwnerOf(vertex));

			auto index = vertex.index;
			vertex = std::move(vertices.back());
			vertex.index = index;
			vertices.pop_back();
		}

		void Graph::removeFromSearchTable(const Vertex& vertex)
		{
			assert(isOwnerOf(vertex));

			verticesSearchTable.remove(vertex.id);
		}

		void Graph::removeVertex(const String& ID)
		{
			removeVertex(getVertex(ID));
		}

		void Graph::removeVertex(Vertex& vertex)
		{
			assert(isOwnerOf(vertex));

			removeEdgesEndingIn(vertex);
			removeFromSearchTable(vertex);
			removeFromVertices(vertex);
		}

		//
		//the default implementation is for an undirected graph:
		//each of the vertex' neighbours has an edge to it
		//
		void Graph::removeEdgesEndingIn(Vertex& vertex)
		{
			for (auto& edge : edgesOf(vertex))
			{
				auto& neighbour = edge.getIncidentVertex();
				removeEdgeFromToNoThrow(neighbour, vertex);
			}
		}

		void Graph::removeEdgeFromToNoThrow(Vertex& from, Vertex& to)
		{
			removeEdgeFromTo(from, to, false);
		}

		void Graph::removeEdgeFromTo(Vertex& from, Vertex& to)
		{
			removeEdgeFromTo(from, to, true);
		}

		void Graph::removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist)
		{
			auto iterator = searchEdgeFromTo(from, to);

			if (iterator)
			{
				auto& edges = edgesOf(from);
				edges.removeAt(iterator);
			}
			else if (throwIfEdgeDoesNotExist)
			{
				throw Exception{ "No such edge exists" };
			}
		}

		Graph::IncidentEdgesIterator Graph::searchEdgeFromTo(Vertex& from, Vertex& to)
		{
			using std::begin;
			using std::end;
			return std::find_if(begin(edgesOf(from)), end(edgesOf(from)),
				[&](const IncidentEdge& edge)
			{
				return edge.getIncidentVertex() == to;
			});
		}

		void Graph::insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight weight)
		{
			try
			{
				auto& edges = edgesOf(from);
				edges.insert(IncidentEdge{ to, weight });
			}
			catch (std::bad_alloc&)
			{
				throw NoMemoryAvailable{};
			}
		}

		bool Graph::existsEdgeFromTo(Vertex& from, Vertex& to)
		{
			auto iterator = searchEdgeFromTo(from, to);

			return static_cast<bool>(iterator);
		}

		bool Graph::isOwnerOf(const Vertex& vertex) const
		{
			auto index = vertex.index;
			return (index < vertices.size()) && (vertices[index] == vertex);
		}

		const Graph::Vertex& Graph::getVertex(const String& ID) const
		{
			auto result = verticesSearchTable.search(ID);

			if (result != nullptr)
			{
				return *result;
			}
			else
			{
				throw Exception{ "There is no vertex with ID " + ID };
			}
		}

		Graph::Vertex& Graph::getVertex(const String& ID)
		{
			return const_cast<Vertex&>( static_cast<const Graph&>(*this).getVertex(ID) );
		}

		auto Graph::getIteratorToEdgesLeaving(Vertex& vertex) -> IncidentEdgeIteratorPtr
		{
			assert(isOwnerOf(vertex));

			using std::begin;
			return makeWrapper(begin(edgesOf(vertex)));
		}

		auto Graph::getConstIteratorToEdgesLeaving(const Vertex& vertex) const -> IncidentEdgeConstIteratorPtr
		{
			assert(isOwnerOf(vertex));

			using std::cbegin;
			return makeWrapper(cbegin(edgesOf(vertex)));
		}

		Graph::VertexConstIteratorPtr Graph::getIteratorToVertices() const
		{
			using std::cbegin;
			using std::cend;
			return makeWrapper(cbegin(vertices), cend(vertices));
		}

		const String& Graph::getID() const
		{
			return id;
		}

		std::uint32_t Graph::getVerticesCount() const
		{
			return vertices.size();
		}

		auto Graph::edgesOf(const Vertex& v) -> const EdgeList&
		{
			auto& iterator = v.edges;
			return *iterator;
		}

		auto Graph::edgesOf(Vertex& v) -> EdgeList&
		{
			return const_cast<EdgeList&>( edgesOf(static_cast<const Vertex&>(v)) );
		}
	}
}