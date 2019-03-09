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

			template <typename Callable, typename T>
			using EnableIfReads = std::enable_if_t<readsValue<Callable, T>>;

			template <typename Callable, typename T>
			using EnableIfModifiesLvalue = std::enable_if_t<modifiesLvalue<Callable, T>>;

			template <typename IteratorPtr, typename Callable>
			inline void forEach(IteratorPtr ptr, Callable f)
			{
				PolymorphicRanges::forEach(*ptr, f);
			}
		}

		template <typename Callable,
			      typename = Detail::EnableIfReads<Callable, Graph::Vertex>>
		inline void forEachVertex(const Graph& g, Callable f) 
		{
			Detail::forEach(g.getConstIteratorToVertices(), f);
		}

		template <typename Callable,
			      typename = Detail::EnableIfModifiesLvalue<Callable, Graph::Vertex>>
		inline void forEachVertex(Graph& g, Callable f)
		{
			Detail::forEach(g.getIteratorToVertices(), f);
		}

		template <typename Callable,
			      typename = Detail::EnableIfReads<Callable, Graph::IncidentEdge>>
		inline void forEachIncidentEdge(const Graph& g, const Graph::Vertex& v, Callable f)
		{
			Detail::forEach(g.getConstIteratorToEdgesLeaving(v), f);
		}

		template <typename Callable,
			      typename = Detail::EnableIfModifiesLvalue<Callable, Graph::IncidentEdge>>
		inline void forEachIncidentEdge(Graph& g, Graph::Vertex& v, Callable f)
		{
			Detail::forEach(g.getIteratorToEdgesLeaving(v), f);
		}

		template <typename Callable>
		inline void forEachEdge(const Graph& g, Callable f)
		{
			static_assert(Detail::readsValue<Callable, Graph::Edge>, "Trying to modify edges of a const Graph");
			Detail::forEach(g.getConstIteratorToEdges(), f);
		}
	}
}

#endif //__GRAPH_UTILS_H_INCLUDED__