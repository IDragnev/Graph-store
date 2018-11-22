#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "..\..\Containers\Hash\Hash.h"
#include "..\..\Containers\Hash\HashFunctionStringSpecialization.h"
#include "..\..\String\String.h"
#include "..\..\Containers\Singly Linked List\SinglyLinkedList.h"
#include "..\..\Iterator abstraction\Iterator.h"
#include "..\..\ForwardIterator wrapper\ForwardIteratorWrapper.h"
#include <memory>
#include <vector>

namespace IDragnev
{
	namespace GraphStore
	{
		class Graph
		{
		public:
			class Vertex;
			class IncidentEdge
			{
			private:
				using VertexRef = std::reference_wrapper<Vertex>;

			public:
				using Weight = std::uint32_t;

				IncidentEdge(Vertex& incidentVertex, Weight);

				Vertex& getIncidentVertex();
				const Vertex& getIncidentVertex() const;
				Weight getWeight() const;

			private:
				VertexRef incidentVertex;
				Weight weight;
			};

		private:
			using EdgeList = Containers::SinglyLinkedList<IncidentEdge>;
			using EdgeListsCollection = Containers::SinglyLinkedList<EdgeList>;

		public:
			class Vertex
			{
			private:
				friend class Graph;
				using EdgeListIterator = EdgeListsCollection::iterator;

			public:
				const String& ID() const { return id; }

			private:
				Vertex(const String& ID, std::size_t position, EdgeListIterator edgesPosition);

				void setID(const String& ID);

			private:
				String id;
				std::size_t position;
				EdgeListIterator edgesPosition;
			};

			class Edge
			{
			private:
				using ConstVertexRef = std::reference_wrapper<const Vertex>;
				using ConstIncidentEdgeRef = std::reference_wrapper<const IncidentEdge>;

			public:
				using Weight = IncidentEdge::Weight;
				
				const Vertex& start() const;
				const Vertex& end() const;
				Weight weight() const;

			private:
				Edge(const Vertex& start, const IncidentEdge& edge); 

			private:
				ConstVertexRef startVertex;
				ConstIncidentEdgeRef incidentEdge;
			};

		private:
			struct IDAccessor
			{
				template <typename T>
				const String& operator()(const T& item) const
				{
					return item.ID();
				}
			};

			template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
			class EdgeIterator
			{
				friend class Graph;
			public:
				const Edge operator*() const;
				EdgeIterator& operator++();
				operator bool() const;
				bool operator!() const;

			private:
				EdgeIterator(VertexForwardIterator vertexIt, IncidentEdgeForwardIterator edgeIt);

			private:
				void toFirstEdge();
				void skipPassedEdges();
				void toNextEdge();
				bool wasCurrentEdgePassed() const;
				void toNextVertex();
				void updateEdgeIterator();

			private:
				VertexForwardIterator vertexIterator;
				IncidentEdgeForwardIterator edgeIterator;
			};

			using VertexArray = std::vector<Vertex>;
			using VertexHashTable = Containers::Hash<Vertex, String, IDAccessor>;
			using IncidentEdgesIterator = EdgeList::iterator;
			using IncidentEdgesConstIterator = EdgeList::const_iterator;
			using VertexConstIterator = 
				Iterators::ForwardIteratorWrapper<VertexArray::const_iterator, VertexArray::const_iterator>;

			using VertexIteratorPtr = std::unique_ptr<Iterators::Iterator<Vertex>>;
			using VertexConstIteratorPtr = std::unique_ptr<Iterators::ConstIterator<Vertex>>;
			using IncidentEdgeIteratorPtr = std::unique_ptr<Iterators::Iterator<IncidentEdge>>;
			using IncidentEdgeConstIteratorPtr = std::unique_ptr<Iterators::ConstIterator<IncidentEdge>>;

		public:
			using UniqueEdgesConstIterator = EdgeIterator<VertexConstIterator, IncidentEdgesConstIterator>;

			Graph(const String& ID);
			Graph(const Graph&) = delete;
			virtual ~Graph() = default;

			Graph& operator=(const Graph&) = delete;

			void insertVertexWithID(const String& ID);
			void removeVertex(const String& ID);
			void removeVertex(Vertex& v);

			virtual void insertEdge(Vertex& start, Vertex& end, Edge::Weight = 1) = 0; 
			virtual void removeEdge(Vertex& start, Vertex& end) = 0;

			Vertex& getVertex(const String& ID);
			const Vertex& getVertex(const String& ID) const;

			std::uint32_t getVerticesCount() const;
			VertexConstIteratorPtr getConstIteratorToVertices() const;
			VertexIteratorPtr getIteratorToVertices();
			IncidentEdgeIteratorPtr getIteratorToEdgesLeaving(Vertex& v);
			IncidentEdgeConstIteratorPtr getConstIteratorToEdgesLeaving(const Vertex& v) const;
			UniqueEdgesConstIterator getUniqueEdgesConstIterator() const;
			const String& getID() const;

		protected:
			virtual void removeEdgesEndingIn(Vertex& v) = 0;
			static void removeEdgeFromToNoThrow(Vertex& from, Vertex& to);
			static void removeEdgeFromTo(Vertex& from, Vertex& to);
			static void insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight);
			static bool existsEdgeFromTo(Vertex& from, Vertex& to);

			bool hasVertexWithID(const String& ID) const;
			bool isOwnerOf(const Vertex& v) const;

		private:
			static void removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist);
			static IncidentEdgesIterator searchEdgeFromTo(Vertex& from, Vertex& to);

			static const EdgeList& edgesOf(const Vertex& v);
			static EdgeList& edgesOf(Vertex& v);

			void tryToInsertVertexWithID(const String& ID);
			void insertInSearchTable(Vertex& v);
			void removeFromSearchTable(const Vertex& v);
			void removeFromVertices(Vertex& v);
			void removeEdgeListOf(Vertex& v);

			EdgeListsCollection::iterator newestEdgeListPosition();
			Vertex& newestVertex();

			void makeVertex(const String& ID);
			void makeEmptyEdgeList();

			void setID(const String& ID);

		private:
			static const std::size_t FEWEST_VERTICES_EXPECTED = 32;

		private:
			String id;
			VertexArray vertices;
			VertexHashTable verticesSearchTable;
			EdgeListsCollection edgeLists;
		};

		bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
		bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
	}
}

#include "EdgeIteratorImpl.hpp"
#endif // __BASE_GRAPH_H_INCLUDED__