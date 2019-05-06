#ifndef __DIRECTED_GRAPH_H_INCLUDED__
#define __DIRECTED_GRAPH_H_INCLUDED__

#include "Graph\Base Graph\Graph.h"

namespace IDragnev::GraphStore
{
	class DirectedGraph : public Graph
	{
	private:
		class DirectedEdgeConstIterator : public EdgeConstIteratorBase
		{
			using Base = EdgeConstIteratorBase;

		public:
			using Base::EdgeConstIterator;

		private:
			bool wasCurrentEdgeIterated() const override;
		};

	public:
		using Graph::Graph;

		void insertEdge(Vertex& start, Vertex& end, Edge::Weight) override;
		void removeEdge(Vertex& start, Vertex& end) override;
		String getType() const override;
		std::size_t getEdgesCount() const noexcept override;
		EdgeConstIteratorPtr getConstIteratorToEdges() const override;

	private:
		void removeEdgesEndingIn(Vertex& v) override;
	};
}

#endif //__DIRECTED_GRAPH_H_INCLUDED__