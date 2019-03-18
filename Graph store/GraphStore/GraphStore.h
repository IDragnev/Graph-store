#ifndef __GRAPH_STORE_H_INCLUDED__
#define __GRAPH_STORE_H_INCLUDED__

#include "..\Containers\Dynamic Array\DArray.h"

namespace IDragnev
{
	class String;

	namespace GraphStore
	{
		class Graph;
		class Exception;

		class GraphStore
		{
		private:
			using GraphPtr = std::unique_ptr<Graph>;
			using GraphCollection = Containers::DArray<GraphPtr>;
			using ConstIterator = GraphCollection::const_iterator;

		public:
			GraphStore() = default;
			GraphStore(const GraphStore&) = delete;
			GraphStore(GraphStore&& source) = default;
			~GraphStore() = default;

			GraphStore& operator=(const GraphStore&) = delete;
			GraphStore& operator=(GraphStore&& rhs) = default;

			void insertGraph(GraphPtr graph);
			void removeGraph(const String& ID);
			Graph& getGraph(const String& ID);
			const Graph& getGraph(const String& ID) const;

			bool isEmpty() const noexcept;
			void empty() noexcept;

		private:
			GraphPtr& getGraphPtr(const String& ID);
			const GraphPtr& getGraphPtr(const String& ID) const;
			bool hasGraphWithID(const String& ID) const;
			ConstIterator searchGraph(const String& ID) const;

		private:
			GraphCollection graphs;
		};
	}
}

#endif //__GRAPH_STORE_H_INCLUDED__