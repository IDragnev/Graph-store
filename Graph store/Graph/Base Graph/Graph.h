#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "Exceptions\Exceptions.h"
#include "Containers\Hash\Hash.h"
#include "HashFunction\HashFunction.h"
#include "String\String.h"
#include "Containers\Singly Linked List\SinglyLinkedList.h"
#include "Iterator abstraction\Iterator.h"
#include "ForwardIterator adaptor\ForwardIteratorAdaptor.h"
#include "IdGetter\IdGetter.h"
#include <memory>
#include <vector>

namespace IDragnev::GraphStore
{
	class Graph
	{
	protected:
		class DuplicateEdge : public Exception
		{
		public:
			DuplicateEdge(const String& start, const String& end);
		};

	public:
		class Vertex;
		class IncidentEdge
		{
		private:
			using VertexRef = std::reference_wrapper<Vertex>;

		public:
			using Weight = std::uint64_t;

			IncidentEdge(Vertex& incidentVertex, Weight) noexcept;

			Vertex& getIncidentVertex() noexcept;
			const Vertex& getIncidentVertex() const noexcept;
			Weight getWeight() const noexcept;

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
			Vertex(const Vertex&) = default;
			~Vertex() = default;

			Vertex& operator=(const Vertex&) = default;

			void swap(Vertex& other) noexcept;

			const String& getID() const noexcept { return id; }

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

			Edge(const Vertex& start, const IncidentEdge& edge) noexcept;

			const Vertex& start() const noexcept;
			const Vertex& end() const noexcept;
			Weight weight() const noexcept;

		private:
			ConstVertexRef startVertex;
			ConstIncidentEdgeRef incidentEdge;
		};

	private:
		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		class EdgeConstIterator
		{
			friend class Graph;
		public:
			EdgeConstIterator(VertexForwardIterator vertexIt, IncidentEdgeForwardIterator edgeIt);
			virtual ~EdgeConstIterator() = default;

			const Edge operator*() const;
			EdgeConstIterator& operator++();
			operator bool() const;
			bool operator!() const;

		private:
			void toFirstEdge();
			void skipIteratedEdges();
			void toNextEdge();
			virtual bool wasCurrentEdgeIterated() const;
			void toNextVertex();
			void updateEdgeIterator();

		private:
			VertexForwardIterator vertexIterator;
			IncidentEdgeForwardIterator edgeIterator;
		};

		using VertexArray = std::vector<Vertex>;
		using VertexHashTable = Containers::Hash<Vertex*, String, IdGetter>;
		using IncidentEdgesIterator = EdgeList::iterator;
		using IncidentEdgesConstIterator = EdgeList::const_iterator;
		using VertexConstIterator =
			ForwardIteratorAdaptor<VertexArray::const_iterator, VertexArray::const_iterator>;
		using VertexIteratorPtr = std::unique_ptr<Iterator<Vertex>>;
		using VertexConstIteratorPtr = std::unique_ptr<ConstIterator<Vertex>>;
		using IncidentEdgeIteratorPtr = std::unique_ptr<Iterator<IncidentEdge>>;
		using IncidentEdgeConstIteratorPtr = std::unique_ptr<ConstIterator<IncidentEdge>>;

	protected:
		using EdgeConstIteratorBase = EdgeConstIterator<VertexConstIterator, IncidentEdgesConstIterator>;
		using EdgeConstIteratorPtr = std::unique_ptr<EdgeConstIteratorBase>;

	public:
		Graph(const String& ID);
		Graph(const Graph&) = delete;
		virtual ~Graph() = default;

		Graph& operator=(const Graph&) = delete;

		void insertVertexWithID(const String& ID);
		void removeVertex(const String& ID);
		void removeVertex(Vertex& v);

		virtual void insertEdge(Vertex& start, Vertex& end, Edge::Weight = 1) = 0;
		virtual void removeEdge(Vertex& start, Vertex& end) = 0;

		virtual String getType() const = 0;

		Vertex& getVertex(const String& ID);
		const Vertex& getVertex(const String& ID) const;
		bool hasVertexWithID(const String& ID) const noexcept;

		std::size_t getVerticesCount() const noexcept;
		virtual std::size_t getEdgesCount() const noexcept = 0;

		VertexConstIteratorPtr getConstIteratorToVertices() const;
		VertexIteratorPtr getIteratorToVertices();
		IncidentEdgeIteratorPtr getIteratorToEdgesLeaving(Vertex& v);
		IncidentEdgeConstIteratorPtr getConstIteratorToEdgesLeaving(const Vertex& v) const;
		virtual EdgeConstIteratorPtr getConstIteratorToEdges() const = 0;
		const String& getID() const noexcept;

	protected:
		virtual void removeEdgesEndingIn(Vertex& v) = 0;
		static void removeEdgeFromToNoThrow(Vertex& from, Vertex& to);
		static void removeEdgeFromTo(Vertex& from, Vertex& to);
		static void insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight);
		static bool existsEdgeFromTo(Vertex& from, Vertex& to);

		bool isOwnerOf(const Vertex& v) const noexcept;

		template <typename ConcreteIterator = EdgeConstIteratorBase>
		EdgeConstIteratorPtr makeEdgeConstIterator() const;

	private:
		template <bool throwIfMissing>
		static void removeEdgeFromTo(Vertex& from, Vertex& to);
		static IncidentEdgesIterator searchEdgeFromTo(Vertex& from, Vertex& to);

		static const EdgeList& edgesOf(const Vertex& v) noexcept;
		static EdgeList& edgesOf(Vertex& v) noexcept;

		void tryToInsertVertexWithID(const String& ID);
		void insertInSearchTable(Vertex& v);
		void removeFromSearchTable(const Vertex& v);
		void removeFromVertices(Vertex& v);
		void updatePositionInSearchTable(Vertex& v);
		void removeEdgeListOf(Vertex& v);

		EdgeListsCollection::iterator newestEdgeListPosition() noexcept;
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

	void swap(Graph::Vertex& lhs, Graph::Vertex& rhs) noexcept;
	bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs) noexcept;
	bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs) noexcept;
}

#include "EdgeConstIteratorImpl.hpp"
#endif // __BASE_GRAPH_H_INCLUDED__