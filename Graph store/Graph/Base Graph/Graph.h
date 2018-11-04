#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "..\..\Containers\Hash\Hash.h"
#include "..\..\Containers\Hash\HashFunctionStringSpecialization.h"
#include "..\..\Containers\Dynamic Array\DArray.h"
#include "..\..\String\String.h"
#include "..\..\Containers\Singly Linked List\SinglyLinkedList.h"
#include "..\..\Iterator abstraction\Iterator.h"
#include <memory>

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

			class Vertex
			{
			private:
				friend class Graph;
				using EdgeList = Containers::SinglyLinkedList<IncidentEdge>;

			public:
				Vertex(const String& ID, std::size_t index);
				Vertex(const Vertex&) = delete;

				Vertex& operator=(const Vertex&) = delete;

				const String& getID() const;

			private:
				void setID(const String& ID);

			private:
				String id;
				std::size_t index;
				EdgeList edges;
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
					return item.getID();
				}
			};

			template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
			class EdgeIterator
			{
			public:
				EdgeIterator(VertexForwardIterator vertexIt, IncidentEdgeForwardIterator edgeIt);

				const Edge operator*() const;
				EdgeIterator& operator++();
				operator bool() const;
				bool operator!() const;

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

			using VertexArray = Containers::DArray<Vertex*>;
			using VertexHashTable = Containers::Hash<Vertex, String, IDAccessor>;
			using IncidentEdgesIterator = Containers::SinglyLinkedList<IncidentEdge>::iterator;
			using VertexConstIteratorPtr = std::unique_ptr<Iterators::ConstIterator<Vertex*>>;
			using IncidentEdgeIteratorPtr = std::unique_ptr<Iterators::Iterator<IncidentEdge>>;
			using IncidentEdgeConstIteratorPtr = std::unique_ptr<Iterators::ConstIterator<IncidentEdge>>;

		public:
			Graph(const String& ID);
			Graph(const Graph&) = delete;
			virtual ~Graph();

			Graph& operator=(const Graph&) = delete;

			void insertVertexWithID(const String& ID);
			void removeVertex(const String& ID);
			void removeVertex(Vertex& vertex);

			virtual void insertEdge(Vertex& start, Vertex& end, Edge::Weight = 1) = 0; 
			virtual void removeEdge(Vertex& start, Vertex& end) = 0;

			Vertex& getVertex(const String& ID);
			const Vertex& getVertex(const String& ID) const;

			std::uint32_t getVerticesCount() const;
			VertexConstIteratorPtr getIteratorToVertices() const;
			IncidentEdgeIteratorPtr getIteratorToEdgesLeaving(Vertex& vertex);
			IncidentEdgeConstIteratorPtr getConstIteratorToEdgesLeaving(const Vertex& vertex) const;

			const String& getID() const;

		protected:
			virtual void removeEdgesEndingIn(Vertex& vertex) = 0;
			static void removeEdgeFromToNoThrow(Vertex& from, Vertex& to);
			static void removeEdgeFromTo(Vertex& from, Vertex& to);
			static void insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight);
			static bool existsEdgeFromTo(Vertex& from, Vertex& to);

			bool hasVertexWithID(const String& ID) const;
			bool isOwnerOf(const Vertex& vertex) const;

		private:
			static void removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist);
			static IncidentEdgesIterator searchEdgeFromTo(Vertex& from, Vertex& to);
		
			void tryToInsertVertexWithID(const String& ID);
			void insert(Vertex& vertex);
			void insertInVertices(Vertex& vertex);
			void insertInSearchTable(Vertex& vertex);
			void removeFromVertices(const Vertex& vertex);
			void removeFromSearchTable(const Vertex& vertex);

			std::unique_ptr<Vertex> createVertex(const String& ID) const;
			static void deleteVertex(Vertex* vertex);

			void setID(const String& ID);

		private:
			static const std::size_t FEWEST_VERTICES_EXPECTED = 32;

		private:
			String id;
			VertexArray vertices;
			VertexHashTable verticesSearchTable;
		};

		bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
		bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
	}
}

#include "EdgeIteratorImpl.hpp"
#endif // __BASE_GRAPH_H_INCLUDED__