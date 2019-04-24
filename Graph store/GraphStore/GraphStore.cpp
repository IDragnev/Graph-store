#include "GraphStore.h"
#include "..\String\String.h"
#include "..\Graph\Base Graph\Graph.h"
#include "Exceptions\Exceptions.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "ID matcher\IDMatcher.h"
#include <algorithm>

namespace IDragnev
{
	namespace GraphStore
	{
		class DuplicateGraphID : public Exception
		{
		public:
			DuplicateGraphID(const String& ID) :
				Exception{ fmt::format("A graph with ID {0} already exists", ID) }
			{
			}
		};

		class NoSuchGraph : public Exception
		{
		public:
			NoSuchGraph(const String& ID) :
				Exception{ fmt::format("No graph with ID {0} exists", ID) }
			{
			}
		};

		void GraphStore::insertGraph(GraphPtr graph)
		{
			if (!hasGraphWithID(graph->getID()))
			{
				graphs.insertBack(std::move(graph));
			}
			else
			{
				throw DuplicateGraphID{ graph->getID() };
			}
		}

		bool GraphStore::hasGraphWithID(const String& ID) const
		{
			return static_cast<bool>(searchGraph(ID));
		}

		auto GraphStore::searchGraph(const String& ID) const -> ConstIterator
		{
			using GraphID = ConstStringIDRef;
			return std::find_if(std::begin(graphs), std::end(graphs), matches(GraphID(ID)));
		}

		Graph& GraphStore::getGraph(const String& ID)
		{
			return const_cast<Graph&>( static_cast<const GraphStore&>(*this).getGraph(ID) );
		}

		const Graph& GraphStore::getGraph(const String& ID) const
		{
			auto& ptr = getGraphPtr(ID);
			return *ptr;
		}

		auto GraphStore::getGraphPtr(const String& ID) -> GraphPtr&
		{
			return const_cast<GraphPtr&>( static_cast<const GraphStore&>(*this).getGraphPtr(ID) );
		}

		auto GraphStore::getGraphPtr(const String& ID) const -> const GraphPtr&
		{
			if (auto iterator = searchGraph(ID); 
				iterator)
			{
				return *iterator;
			}
			else
			{
				throw NoSuchGraph{ ID };
			}
		}

		void GraphStore::removeGraph(const String& ID)
		{
			auto& graphPtr = getGraphPtr(ID);		
			auto last = graphs.getCount() - 1;
			graphPtr = std::move(graphs[last]);
			graphs.removeAt(last);
		}

		bool GraphStore::isEmpty() const noexcept
		{
			return graphs.isEmpty();
		}

		void GraphStore::empty() noexcept
		{
			graphs.clear();
		}
	}
}