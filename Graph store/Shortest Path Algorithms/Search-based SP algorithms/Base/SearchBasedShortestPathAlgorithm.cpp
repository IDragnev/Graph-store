#include "SearchBasedShortestPathAlgorithm.h"

namespace IDragnev
{
	namespace GraphStore
	{
		void SearchBasedShortestPathAlgorithm::decorate(const Graph& graph, const Vertex& source)
		{
			assert(decorators.empty());

			auto iteratorPtr = graph.getConstIteratorToVertices();

			forEach(*iteratorPtr, [&](const Vertex& v)
			{
				decorators.try_emplace(v.ID(), &v);
			});

			initSourceDecorator(decoratorOf(source));
		}

		void SearchBasedShortestPathAlgorithm::initSourceDecorator(MarkableVertex& source)
		{
			source.distance = 0;
			source.predecessor = nullptr;
			source.isVisited = true;
		}

		auto SearchBasedShortestPathAlgorithm::decoratorOf(const Vertex& v) -> MarkableVertex&
		{
			return const_cast<MarkableVertex&>(
					static_cast<const SearchBasedShortestPathAlgorithm&>(*this).decoratorOf(v));
		}

		auto SearchBasedShortestPathAlgorithm::decoratorOf(const Vertex& v) const -> const MarkableVertex&
		{
			auto iterator = decorators.find(v.ID());
			assert(iterator != decorators.cend());

			auto& pair = *iterator;
			return std::get<1>(pair);
		}

		void SearchBasedShortestPathAlgorithm::cleanDecoratedState()
		{
			decorators.clear();
		}
	}
}