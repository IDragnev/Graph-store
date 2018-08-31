#include "GraphStore.h"
#include "..\String\String.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include <algorithm>


GraphStore::~GraphStore()
{
	deleteAllGraphs();
}


void GraphStore::deleteAllGraphs()
{
	for (auto&& g : graphs)
	{
		delete g;
	}
}


GraphStore& GraphStore::operator=(GraphStore&& rhs)
{
	if (this != &rhs)
	{
		deleteAllGraphs();
		graphs = std::move(rhs.graphs);
	}

	return *this;
}


void GraphStore::insertGraph(Graph& graph)
{
	if (!hasGraphWithID(graph.getID()))
	{
		graphs.insert(&graph);
	}
	else
	{
		throw Exception{ "A graph with ID \'" + graph.getID() + "\' already exists" };
	}
}


bool GraphStore::hasGraphWithID(const String& ID) const
{
	return searchGraph(ID) != nullptr;
}


const Graph* GraphStore::searchGraph(const String& ID) const
{
	using namespace std;
	auto iterator = std::find_if(cbegin(graphs), cend(graphs), [&](const Graph* g)
	{
		return g->getID() == ID;
	});

	return (iterator) ? *iterator : nullptr;
}


Graph& GraphStore::getGraph(const String& ID)
{
	return const_cast<Graph&>( static_cast<const GraphStore&>(*this).getGraph(ID) );
}


const Graph& GraphStore::getGraph(const String& ID) const
{
	const Graph* result = searchGraph(ID);

	if (result)
	{
		return *result;
	}
	else
	{
		throwNonExistingGraph(ID);
	}
}


void GraphStore::throwNonExistingGraph(const String& ID)
{
	throw Exception{ "No graph with ID \'" + ID + "\' exists" };
}


void GraphStore::removeGraph(const String& ID)
{
	auto i = std::size_t{0};
	for (auto&& g : graphs)
	{
		if (g->getID() == ID)
		{
			removeGraphAt(i);
			return;
		}

		++i;
	}

	throwNonExistingGraph(ID);
}


void GraphStore::removeGraphAt(std::size_t index)
{
	auto* toRemove = graphs[index];
	auto lastGraphIndex = graphs.getCount() - 1;

	std::swap(graphs[index], graphs[lastGraphIndex]);
	graphs.removeAt(lastGraphIndex);

	delete toRemove;
}


bool GraphStore::isEmpty() const
{
	return graphs.isEmpty();
}


void GraphStore::empty()
{
	deleteAllGraphs();
	graphs.empty();
}