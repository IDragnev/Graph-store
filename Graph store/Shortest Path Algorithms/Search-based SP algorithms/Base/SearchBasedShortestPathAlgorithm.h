#ifndef __SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__
#define __SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__

#include "..\..\Base\ShortestPathAlgorithm.h"
#include "..\..\..\String\String.h"
#include <unordered_map>

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
				bool isVisited = false;
			};

		private:
			using ConstStringRef = std::reference_wrapper<const String>;
			using VertexMap =
				std::unordered_map<ConstStringRef, MarkableVertex, std::hash<String>, std::equal_to<String>>;

		public:
			using ShortestPathAlgorithm::ShortestPathAlgorithm;

		protected:
			void decorate(const Graph& graph, const Vertex& source);
			virtual void initSourceDecorator(MarkableVertex& source) = 0;
			void cleanDecoratedState();

			MarkableVertex& decoratorOf(const Vertex& v);
			const MarkableVertex& decoratorOf(const Vertex& v) const;

		private:
			VertexMap decorators{};
		};
	}
}

#endif //__SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__