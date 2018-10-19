#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "..\..\String\String.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\Special Integer\specialInteger.h"
#include "..\..\Containers\Singly Linked List\SinglyLinkedList.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class ShortestPathAlgorithm
		{
		protected:
			using Edge = Graph::Edge;
			using Vertex = Graph::Vertex;
			using Distance = SpecialInteger<unsigned>;
			using EdgeConstIteratorPtr = std::unique_ptr<Iterators::ConstIterator<Edge>>;

			struct VertexDecorator
			{
				VertexDecorator(const Vertex* v) : vertex{ v } {}

				const Vertex* vertex = nullptr;
				const VertexDecorator* predecessor = nullptr;
				Distance distance = Distance::Infinity();
			};

		public:
			class Path
			{
			private:
				using Collection = Containers::SinglyLinkedList<String>;

			public:
				Path() = default;
				Path(Path&& source);
				Path(const Path& source) = default;
				Path(const VertexDecorator& last);

				Path& operator=(Path&& rhs);
				Path& operator=(const Path& rhs) = default;

				void print() const;
				const Distance& getLength() const;

			private:
				Collection IDs;
				Distance length = Distance::Infinity();
			};

		public:
			explicit ShortestPathAlgorithm(const String& ID);
			virtual ~ShortestPathAlgorithm() = default;

			ShortestPathAlgorithm(const ShortestPathAlgorithm&) = delete;
			ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = delete;

			Path findShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal);

			const String& getID() const;

		protected:
			bool isTheGoal(const VertexDecorator& v) const;
			EdgeConstIteratorPtr getEdgesLeaving(const VertexDecorator& v) const;

		private:
			virtual Path findNonTrivialShortestPath(const Graph& graph,
													const Vertex& source,
													const Vertex& goal) = 0;
			static Path buildTrivialPath(const Vertex& v);

			void init(const Graph& graph, const Vertex& goal);

		private:
			const String id;
			const Graph* searchedGraph;
			const Vertex* goal;
		};
	}
}

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
