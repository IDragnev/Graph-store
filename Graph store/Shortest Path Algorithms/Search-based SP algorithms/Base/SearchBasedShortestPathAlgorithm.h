#ifndef __SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__
#define __SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__

#include "..\..\Base\ShortestPathAlgorithm.h"
#include "..\..\..\String\String.h"
#include "..\..\..\Containers\Hash\Hash.h"
#include "..\..\..\Containers\Dynamic Array\DArray.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class SearchBasedShortestPathAlgorithm : public ShortestPathAlgorithm
		{
		protected:
			struct MarkableVertex : VertexDecorator
			{
				using VertexDecorator::VertexDecorator;
				MarkableVertex() noexcept : VertexDecorator{ nullptr } {}
				bool isVisited = false;
			};

		private:
			struct KeyAccessor
			{
				const String& operator()(const MarkableVertex* v) const noexcept { return v->vertex->ID(); }
			};

			using VertexArray = Containers::DArray<MarkableVertex>;
			using VertexPtrMap = Containers::Hash<MarkableVertex*, String, KeyAccessor>;
		
		public:
			using ShortestPathAlgorithm::ShortestPathAlgorithm;

		protected:
			void decorate(const Graph& g, const Vertex& source);
			virtual void initSourceDecorator(MarkableVertex& source) = 0;
			void clear() override;

			MarkableVertex& decoratorOf(const Vertex& v);
			const MarkableVertex& decoratorOf(const Vertex& v) const;

		private:
			void setupCollections(std::size_t verticesCount);
			void decorateVertices(const Graph& g);
			void buildDecoratorsMap();

		private:
			VertexArray decorators{};
			VertexPtrMap map{};
		};
	}
}

#endif //__SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__