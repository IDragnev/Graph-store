#include "GraphStore.h"
#include "String\String.h"
#include "Graph\Base Graph\Graph.h"
#include "Exceptions\Exceptions.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "ID matcher\IDMatcher.h"
#include "Containers\Iterator\ProjectionIterator.h"
#include <algorithm>

namespace IDragnev::GraphStore
{
	using GraphID = ConstStringIDRef;

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

	void GraphStore::insert(GraphPtr graph)
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
		auto it = std::find_if(cbegin(), cend(), matches(GraphID(ID)));
		
		return static_cast<bool>(it);
	}

	void GraphStore::remove(const String& ID)
	{
		auto& graphPtr = getGraphPtr(ID);
		auto last = graphs.getCount() - 1;
		graphPtr = std::move(graphs[last]);
		graphs.removeAt(last);
	}

	auto GraphStore::getGraphPtr(const String& ID) -> GraphPtr&
	{
		if (auto iterator = searchGraphPtr(ID);
			iterator)
		{
			return *iterator;
		}
		else
		{
			throw NoSuchGraph{ ID };
		}
	}

	auto GraphStore::searchGraphPtr(const String& ID) -> GraphCollection::iterator
	{
		return std::find_if(std::begin(graphs), std::end(graphs), matches(GraphID(ID)));
	}

	Graph& GraphStore::operator[](const String& ID)
	{
		return const_cast<Graph&>(static_cast<const GraphStore&>(*this)[ID]);
	}

	const Graph& GraphStore::operator[](const String& ID) const
	{
		if (auto it = std::find_if(begin(), end(), matches(GraphID(ID)));
			it)
		{
			return *it;
		}
		else
		{
			throw NoSuchGraph{ ID };
		}
	}

	bool GraphStore::isEmpty() const noexcept
	{
		return graphs.isEmpty();
	}

	void GraphStore::empty() noexcept
	{
		graphs.clear();
	}

	auto GraphStore::begin() noexcept -> iterator
	{
		return std::begin(graphs);
	}

	auto GraphStore::end() noexcept -> iterator
	{
		return std::end(graphs);
	}

	auto GraphStore::begin() const noexcept -> const_iterator
	{
		return std::cbegin(graphs);
	}

	auto GraphStore::end() const noexcept -> const_iterator
	{
		return std::cend(graphs);
	}

	auto GraphStore::cbegin() const noexcept -> const_iterator
	{
		return begin();
	}

	auto GraphStore::cend() const noexcept -> const_iterator
	{
		return end();
	}
}