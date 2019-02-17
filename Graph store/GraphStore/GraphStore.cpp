#include "GraphStore.h"
#include "..\String\String.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include <algorithm>

namespace IDragnev
{
	namespace GraphStore
	{
		void GraphStore::insertGraph(GraphPtr graph)
		{
			if (!hasGraphWithID(graph->getID()))
			{
				graphs.insert(std::move(graph));
			}
			else
			{
				throw Exception{ "A graph with ID \'" + graph->getID() + "\' already exists" };
			}
		}

		bool GraphStore::hasGraphWithID(const String& ID) const
		{
			return isValid(searchGraph(ID));
		}

		bool GraphStore::isValid(ConstIterator it) const noexcept 
		{
			using std::cend;
			return it != cend(graphs);
		}

		auto GraphStore::searchGraph(const String& ID) const -> ConstIterator
		{
			using std::cbegin;
			using std::cend;
			return std::find_if(cbegin(graphs), cend(graphs), 
				                [&ID](const GraphPtr& g) { return g->getID() == ID; });
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
			if (auto iterator = searchGraph(ID); isValid(iterator))
			{
				return *iterator;
			}
			else
			{
				throw Exception{ "No graph with ID \'" + ID + "\' exists" };
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
			graphs.empty();
		}
	}
}