#ifndef __GRAPH_UTILS_H_INCLUDED__
#define __GRAPH_UTILS_H_INCLUDED__

#include "Graph.h"
#include <type_traits>

namespace IDragnev
{
	namespace GraphStore
	{
		namespace Detail
		{
			template <typename Callable>
			inline static constexpr bool readsVertex = std::is_invocable_v<Callable, const Graph::Vertex>;

			template <typename Callable>
			inline static constexpr bool modifiesLvalueVertex = !readsVertex<Callable> && std::is_invocable_v<Callable, Graph::Vertex&>;

			template <typename Callable>
			using EnableIfReadsVertex = std::enable_if_t<readsVertex<Callable>>;

			template <typename Callable>
			using EnableIfModifiesLvalueVertex = std::enable_if_t<modifiesLvalueVertex<Callable>>;
		}

		template <typename Callable,
			      typename = Detail::EnableIfReadsVertex<Callable>>
		inline void forEachVertex(const Graph& g, Callable f) 
		{
			auto iteratorPtr = g.getConstIteratorToVertices();
			forEach(*iteratorPtr, f);
		}

		template <typename Callable,
			      typename = Detail::EnableIfModifiesLvalueVertex<Callable>>
		inline void forEachVertex(Graph& g, Callable f)
		{
			auto iteratorPtr = g.getIteratorToVertices();
			forEach(*iteratorPtr, f);
		}
	}
}

#endif //__GRAPH_UTILS_H_INCLUDED__