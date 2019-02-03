#include "Graph.h"
#include "..\..\General Exceptions\NoMemoryAvailable.h"
#include <assert.h>
#include <algorithm>

namespace IDragnev
{
	namespace GraphStore
	{
		Graph::IncidentEdge::IncidentEdge(Vertex& incidentVertex, Weight weight) noexcept :
			incidentVertex{ incidentVertex },
			weight{ weight }
		{
		}

		Graph::Vertex& Graph::IncidentEdge::getIncidentVertex() noexcept
		{
			return const_cast<Vertex&>( static_cast<const IncidentEdge&>(*this).getIncidentVertex() );
		}

		const Graph::Vertex& Graph::IncidentEdge::getIncidentVertex() const noexcept
		{
			return incidentVertex;
		}

		auto Graph::IncidentEdge::getWeight() const noexcept -> Weight
		{
			return weight;
		}

		Graph::Vertex::Vertex(String ID, std::size_t position, EdgeListIterator edgesPosition) :
			position{ position },
			edgesPosition{ edgesPosition }
		{
			setID(std::move(ID));
		}

		void Graph::Vertex::setID(String&& ID)
		{
			if (ID != String{ "" })
			{
				id = std::move(ID);
			}
			else
			{
				throw Exception{ "A Vertex ID must be a valid string" };
			}
		}

		void Graph::Vertex::swap(Vertex& other) noexcept
		{
			using std::swap;
			swap(id, other.id);
			swap(position, other.position);
			swap(edgesPosition, other.edgesPosition);
		}

		void swap(Graph::Vertex& lhs, Graph::Vertex& rhs) noexcept
		{
			lhs.swap(rhs);
		}

		bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs) noexcept
		{
			return &lhs == &rhs;
		}

		bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		Graph::Edge::Edge(const Vertex& start, const IncidentEdge& edge) noexcept :
			startVertex{ start },
			incidentEdge{ edge }
		{
		}

		auto Graph::Edge::start() const noexcept -> const Vertex&
		{
			return startVertex;
		}

		auto Graph::Edge::end() const noexcept -> const Vertex&
		{
			return incidentEdge.get().getIncidentVertex();
		}

		auto Graph::Edge::weight() const noexcept -> Weight
		{
			return incidentEdge.get().getWeight();
		}	

		Graph::Graph(String ID) :
			id{},
			vertices{},
			verticesSearchTable{ FEWEST_VERTICES_EXPECTED },
			edgeLists{}
		{
			setID(std::move(ID));
			vertices.reserve(FEWEST_VERTICES_EXPECTED);
		}

		void Graph::setID(String&& ID)
		{
			if (ID != String{ "" })
			{
				id = std::move(ID);
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
				makeEmptyEdgeList();
				makeVertex(ID);
				insertInSearchTable(newestVertex());
			}
			catch (std::bad_alloc&)
			{
				throw NoMemoryAvailable{};
			}
		}

		void Graph::makeEmptyEdgeList()
		{
			edgeLists.insertAsHead({});
		}

		auto Graph::newestVertex() -> Vertex&
		{
			return vertices.back();
		}

		void Graph::makeVertex(const String& ID)
		{
			assert(edgeLists.getCount() == vertices.size() + 1);

			auto position = vertices.size();
			auto edgesPosition = newestEdgeListPosition();

			try
			{
				vertices.push_back({ ID, position, edgesPosition });
			}
			catch (std::bad_alloc&)
			{
				edgeLists.removeAt(edgesPosition);
				throw;
			}
		}

		auto Graph::newestEdgeListPosition() -> EdgeListsCollection::iterator
		{
			using std::begin;
			return begin(edgeLists);
		}  

		void Graph::insertInSearchTable(Vertex& v)
		{
			try
			{
				verticesSearchTable.insert(&v);
			}
			catch (std::bad_alloc&)
			{
				removeEdgeListOf(v);
				removeFromVertices(v);
				throw;
			}
		}

		void Graph::removeEdgeListOf(Vertex& v)
		{
			edgeLists.removeAt(v.edgesPosition);
		}

		void Graph::removeFromVertices(Vertex& v)
		{
			assert(isOwnerOf(v));

			if (v != vertices.back())
			{
				auto position = v.position;
				swap(v, vertices.back());
				v.position = position;
				updatePositionInSearchTable(v);
			}

			vertices.pop_back();
		}

		void Graph::updatePositionInSearchTable(Vertex& v)
		{
			removeFromSearchTable(v);
			insertInSearchTable(v);
		}

		void Graph::removeFromSearchTable(const Vertex& v)
		{
			assert(isOwnerOf(v));
			verticesSearchTable.remove(v.id);
		}

		void Graph::removeVertex(const String& ID)
		{
			removeVertex(getVertex(ID));
		}

		void Graph::removeVertex(Vertex& v)
		{
			assert(isOwnerOf(v));

			removeEdgesEndingIn(v);
			removeEdgeListOf(v);
			removeFromSearchTable(v); 
			removeFromVertices(v);   
		}

		//
		//the default implementation is for an undirected graph:
		//each of the vertex' neighbours has an edge to it
		//
		void Graph::removeEdgesEndingIn(Vertex& v)
		{
			for (auto& edge : edgesOf(v))
			{
				auto& neighbour = edge.getIncidentVertex();
				removeEdgeFromToNoThrow(neighbour, v);
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

		auto Graph::searchEdgeFromTo(Vertex& from, Vertex& to) -> IncidentEdgesIterator
		{
			using std::begin;
			using std::end;

			return std::find_if(begin(edgesOf(from)), end(edgesOf(from)), 
								[&to](const IncidentEdge& edge) { return edge.getIncidentVertex() == to; });
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
			auto position = vertex.position;
			return (position < vertices.size()) && (vertices[position] == vertex);
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
			return makeIteratorWrapper(begin(edgesOf(vertex)));
		}

		auto Graph::getConstIteratorToEdgesLeaving(const Vertex& vertex) const -> IncidentEdgeConstIteratorPtr
		{
			assert(isOwnerOf(vertex));

			using std::cbegin;
			return makeIteratorWrapper(cbegin(edgesOf(vertex)));
		}

		auto Graph::getConstIteratorToVertices() const -> VertexConstIteratorPtr
		{
			using std::cbegin;
			using std::cend;
			return makeIteratorWrapper(cbegin(vertices), cend(vertices));
		}

		auto Graph::getIteratorToVertices() -> VertexIteratorPtr
		{
			using std::begin;
			using std::end;
			return makeIteratorWrapper(begin(vertices), end(vertices));
		}

		const String& Graph::getID() const noexcept
		{
			return id;
		}

		std::uint32_t Graph::getVerticesCount() const
		{
			return vertices.size();
		}

		auto Graph::edgesOf(const Vertex& v) -> const EdgeList&
		{
			auto& iterator = v.edgesPosition;
			return *iterator;
		}

		auto Graph::edgesOf(Vertex& v) -> EdgeList&
		{
			return const_cast<EdgeList&>( edgesOf(static_cast<const Vertex&>(v)) );
		}
	}
}