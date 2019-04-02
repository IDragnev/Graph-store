#ifndef __UNDIRECTED_GRAPH_H_INCLUDED__
#define __UNDIRECTED_GRAPH_H_INCLUDED__

#include "..\Base Graph\Graph.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class UndirectedGraph : public Graph
		{
		public:
			using Graph::Graph;

			void insertEdge(Vertex& start, Vertex& end, Edge::Weight) override;
			void removeEdge(Vertex& start, Vertex& end) override;
			String getType() const override;
			EdgeConstIteratorPtr getConstIteratorToEdges() const override;
			std::size_t getEdgesCount() const noexcept override;

		private:
			void removeEdgesEndingIn(Vertex& vertex) override;
			void insertEdgeInBothDirections(Vertex& start, Vertex& end, Edge::Weight);
		};
	}
}

#endif //__UNDIRECTED_GRAPH_H_INCLUDED__