#ifndef __GRAPH_STORE_H_INCLUDED__
#define __GRAPH_STORE_H_INCLUDED__

#include "Containers\Dynamic Array\DArray.h"

namespace IDragnev
{
	class String;

	namespace Containers
	{
		template <typename Iterator, typename Extractor>
		class ProjectionIterator;
	}
}

namespace IDragnev::GraphStore
{
	class Graph;
	class Exception;

	class GraphStore
	{
	private:
		using GraphPtr = std::unique_ptr<Graph>;
		using GraphCollection = Containers::DArray<GraphPtr>;

		class Extractor
		{
		public:
			const Graph& operator()(GraphCollection::const_iterator it)
			{
				return extractGraph(it);
			}

			Graph& operator()(GraphCollection::iterator it)
			{
				return extractGraph(it);
			}

		private:
			template <typename Iterator>
			static decltype(auto) extractGraph(Iterator it)
			{
				auto& ptr = *it;
				return *ptr;
			}
		};

	public:
		using iterator = Containers::ProjectionIterator<GraphCollection::iterator, Extractor>;
		using const_iterator = Containers::ProjectionIterator<GraphCollection::const_iterator, Extractor>;

		GraphStore() = default;
		GraphStore(const GraphStore&) = delete;
		GraphStore(GraphStore&& source) = default;
		~GraphStore() = default;

		GraphStore& operator=(const GraphStore&) = delete;
		GraphStore& operator=(GraphStore&& rhs) = default;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		Graph& operator[](const String& ID);
		const Graph& operator[](const String& ID) const;

		void insert(GraphPtr graph);
		void remove(const String& ID);

		bool isEmpty() const noexcept;
		void empty() noexcept;

	private:
		bool hasGraphWithID(const String& ID) const;
		GraphPtr& getGraphPtr(const String& ID);
		GraphCollection::iterator searchGraphPtr(const String& ID);

	private:
		GraphCollection graphs;
	};
}

#endif //__GRAPH_STORE_H_INCLUDED__