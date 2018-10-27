
namespace IDragnev
{
	namespace GraphStore
	{
		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::EdgeIterator(VertexForwardIterator vertexIt, EdgeForwardIterator edgeIt) :
			vertexIterator{ vertexIt },
			edgeIterator{ edgeIt }
		{
			assert(!edgeIterator || vertexIterator);
			toFirstEdge();
		}
		
		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline void Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::toFirstEdge()
		{
			skipPassedEdges();
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		void Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::skipPassedEdges()
		{
			while (vertexIterator && wasCurrentEdgePassed());
			{
				toNextEdge();
			}
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		bool Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::wasCurrentEdgePassed() const
		{
			if (!edgeIterator)
			{
				return true;
			}
			else
			{
				auto& currentVertex = *vertexIterator;
				auto& neighbour = (*edgeIterator).getIncidentVertex();

				return currentVertex.index > neighbour.index;
			}
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline void Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::toNextEdge()
		{
			++edgeIterator;
			if (!edgeIterator)
			{
				toNextVertex();
			}
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		void Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::toNextVertex()
		{
			++vertexIterator;
			if (vertexIterator)
			{
				updateEdgeIterator();
			}
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline void Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::updateEdgeIterator()
		{
			using std::begin;
			auto& v = *vertexIterator;
			edgeIterator = begin(Graph::edgesOf(v));
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		auto Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::operator++() -> EdgeIterator&
		{
			++edgeIterator;
			skipPassedEdges();

			return *this;
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline auto Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::operator->() const -> const Edge*
		{
			return &(this->operator*());
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline auto Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::operator*() const -> const Edge&
		{
			assert(this->operator bool());
			return *edgeIterator;
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::operator bool() const
		{
			return vertexIterator && edgeIterator;
		}

		template <typename VertexForwardIterator, typename EdgeForwardIterator>
		inline bool Graph::EdgeIterator<VertexForwardIterator, EdgeForwardIterator>::operator!() const
		{
			return !(this->operator bool());
		}
	}
}