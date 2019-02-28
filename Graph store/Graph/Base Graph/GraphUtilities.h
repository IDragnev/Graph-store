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
			template <typename Callable, typename T>
			inline static constexpr bool readsValue = std::is_invocable_v<Callable, const T>;

			template <typename Callable, typename T>
			inline static constexpr bool modifiesLvalue = !readsValue<Callable, T> && std::is_invocable_v<Callable, T&>;

			template <typename Callable>
			using EnableIfReadsVertex = std::enable_if_t<readsValue<Callable, Graph::Vertex>>;

			template <typename Callable>
			using EnableIfModifiesLvalueVertex = std::enable_if_t<modifiesLvalue<Callable, Graph::Vertex>>;
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