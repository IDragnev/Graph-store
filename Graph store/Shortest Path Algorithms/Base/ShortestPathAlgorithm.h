#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "String\String.h"
#include "Graph\Base Graph\Graph.h"
#include "Graph\Base Graph\GraphUtilities.h"
#include "Special Integer\specialInteger.h"
#include "Containers\Singly Linked List\SinglyLinkedList.h"

namespace IDragnev::GraphStore
{
	class ShortestPathAlgorithm
	{
	protected:
		using IncidentEdge = Graph::IncidentEdge;
		using Vertex = Graph::Vertex;
		using Distance = SpecialInteger<Graph::Edge::Weight>;

		struct VertexDecorator
		{
			VertexDecorator(const Vertex* v) : vertex{ v } {}

			const Vertex* vertex = nullptr;
			const VertexDecorator* predecessor = nullptr;
			Distance distance{};
		};

	public:
		class Path
		{
		private:
			using Collection = Containers::SinglyLinkedList<String>;

		public:
			using Iterator = Collection::const_iterator;

			Path() = default;
			Path(Path&& source);
			Path(const Path& source) = default;
			Path(const VertexDecorator& last);

			Path& operator=(Path&& rhs);
			Path& operator=(const Path& rhs) = default;

			Iterator begin() const { return IDs.begin(); }
			Iterator end() const { return IDs.end(); }
			const Distance& getLength() const;

		private:
			Collection IDs;
			Distance length{};
		};

	public:
		explicit ShortestPathAlgorithm(const String& ID);
		virtual ~ShortestPathAlgorithm() = default;

		ShortestPathAlgorithm(const ShortestPathAlgorithm&) = delete;
		ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = delete;

		Path findShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal);

		const String& getID() const noexcept;

	protected:
		bool isTheGoal(const VertexDecorator& v) const;

		template <typename Callable>
		void forEachIncidentEdgeOf(const VertexDecorator& d, Callable f) const
		{
			forEachIncidentEdge(*searchedGraph, *d.vertex, f);
		}

		template <typename Callable, typename Predicate>
		void forEachIncidentEdgeOfUntil(const VertexDecorator& d, Callable f, Predicate p) const
		{
			forEachIncidentEdgeUntil(*searchedGraph, *d.vertex, f, p);
		}

	private:
		virtual Path findNonTrivialShortestPath(const Graph& graph,
			                                    const Vertex& source,
			                                    const Vertex& goal) = 0;
		virtual void clear() = 0;
		void init(const Graph& graph, const Vertex& goal);
	
		static Path buildTrivialPath(const Vertex& v);

	private:
		const String id;
		const Graph* searchedGraph;
		const Vertex* goal;
	};
}

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
