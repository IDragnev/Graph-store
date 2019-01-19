#include "SearchBasedShortestPathAlgorithm.h"

namespace IDragnev
{
	namespace GraphStore
	{
		void SearchBasedShortestPathAlgorithm::decorate(const Graph& g, const Vertex& source)
		{
			setupCollections(g.getVerticesCount());
			decorateVertices(g);
			buildMapOfDecoratedVertices();
			initSourceDecorator(decoratorOf(source));
		}

		void SearchBasedShortestPathAlgorithm::setupCollections(std::size_t verticesCount)
		{
			assert(decorators.isEmpty());
			assert(map.isEmpty());
			decorators.ensureSize(verticesCount);
			map = VertexPtrMap{ verticesCount };
		}

		void SearchBasedShortestPathAlgorithm::decorateVertices(const Graph& g)
		{
			auto iteratorPtr = g.getConstIteratorToVertices();

			forEach(*iteratorPtr, [&](const Vertex& v)
			{
				decorators.insert({ &v });
			});
		}

		void SearchBasedShortestPathAlgorithm::buildMapOfDecoratedVertices()
		{
			for (auto&& d : decorators)
			{
				map.insert(&d);
			}
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
			auto result = map.search(v.ID());
			assert(result != nullptr);
			return *result;
		}

		void SearchBasedShortestPathAlgorithm::cleanDecoratedState()
		{
			decorators.empty();
			map.empty();
		}
	}
}