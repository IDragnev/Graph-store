#ifndef __DIRECTED_GRAPH_H_INCLUDED__
#define __DIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class DirectedGraph : public Graph
		{
		public:
			using Graph::Graph;

			void insertEdge(Vertex& start, Vertex& end, Edge::Weight) override;
			void removeEdge(Vertex& start, Vertex& end) override;

		private:
			void removeEdgesEndingIn(Vertex& v) override;
		};
	}
}

#endif //__DIRECTED_GRAPH_H_INCLUDED__