
namespace IDragnev::GraphStore
{
	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::EdgeConstIterator(VertexForwardIterator vertexIt, IncidentEdgeForwardIterator edgeIt) :
		vertexIterator{ vertexIt },
		edgeIterator{ edgeIt }
	{
		assert(!edgeIterator || vertexIterator);
		toFirstEdge();
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	inline void Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::toFirstEdge()
	{
		skipIteratedEdges();
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	void Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::skipIteratedEdges()
	{
		while (vertexIterator && wasCurrentEdgeIterated())
		{
			toNextEdge();
		}
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	bool Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::wasCurrentEdgeIterated() const
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

			return currentVertex.position > neighbour.position;
		}
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	inline void Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::toNextEdge()
	{
		++edgeIterator;
		if (!edgeIterator)
		{
			toNextVertex();
		}
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	void Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::toNextVertex()
	{
		++vertexIterator;
		if (vertexIterator)
		{
			updateEdgeIterator();
		}
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	inline void Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::updateEdgeIterator()
	{
		auto& v = *vertexIterator;
		edgeIterator = std::begin(Graph::edgesOf(v));
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	auto Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator++() -> EdgeConstIterator&
	{
		++edgeIterator;
		skipIteratedEdges();

		return *this;
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	inline auto
		Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator*() const -> const Edge
	{
		assert(this->operator bool());
		return { *vertexIterator, *edgeIterator };
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	inline Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator bool() const
	{
		return vertexIterator && edgeIterator;
	}

	template <typename VertexForwardIterator, typename IncidentEdgeForwardIterator>
	inline bool Graph::EdgeConstIterator<VertexForwardIterator, IncidentEdgeForwardIterator>::operator!() const
	{
		return !(this->operator bool());
	}

	template <typename ConcreteIterator>
	auto Graph::makeEdgeConstIterator() const -> EdgeConstIteratorPtr
	{
		using std::cbegin;
		using std::cend;

		auto vertexIt = VertexConstIterator{ cbegin(vertices), cend(vertices) };
		auto edgeIt = vertexIt ? cbegin(edgesOf(*vertexIt)) : cbegin(EdgeList{});

		return std::make_unique<ConcreteIterator>(vertexIt, edgeIt);
	}
}