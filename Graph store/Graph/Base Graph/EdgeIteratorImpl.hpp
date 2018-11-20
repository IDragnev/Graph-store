
namespace IDragnev
{
	namespace GraphStore
	{
		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::EdgeIterator(VertexForwardIterator vertexIt, IncidentEdgeForwardIterator edgeIt) :
			vertexIterator{ vertexIt },
			edgeIterator{ edgeIt }
		{
			assert(!edgeIterator || vertexIterator);
			toFirstEdge();
		}
		
		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		inline void Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::toFirstEdge()
		{
			skipPassedEdges();
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		void Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::skipPassedEdges()
		{
			while (vertexIterator && wasCurrentEdgePassed());
			{
				toNextEdge();
			}
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		bool Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::wasCurrentEdgePassed() const
		{
			if (!edgeIterator)
			{
				return true;
			}
			else
			{
				auto& currentVertex = *vertexIterator;
				auto& currentEdge = *edgeIterator;
				auto& neighbour = currentEdge.getIncidentVertex();

				return currentVertex.index > neighbour.index;
			}
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		inline void Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::toNextEdge()
		{
			++edgeIterator;
			if (!edgeIterator)
			{
				toNextVertex();
			}
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		void Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::toNextVertex()
		{
			++vertexIterator;
			if (vertexIterator)
			{
				updateEdgeIterator();
			}
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		inline void Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::updateEdgeIterator()
		{
			using std::begin;

			auto& v = *vertexIterator;
			edgeIterator = begin(Graph::edgesOf(v));
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		auto Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator++() -> EdgeIterator&
		{
			++edgeIterator;
			skipPassedEdges();

			return *this;
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		inline auto Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator*() const -> const Edge
		{
			assert(this->operator bool());
			return Edge{ *vertexIterator, *edgeIterator };
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		inline Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator bool() const
		{
			return vertexIterator && edgeIterator;
		}

		template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
		inline bool Graph::EdgeIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator!() const
		{
			return !(this->operator bool());
		}
	}
}