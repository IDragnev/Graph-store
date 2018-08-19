#include "GraphStore.h"
#include "..\String\String.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"


GraphStore::~GraphStore()
{
	deleteAllGraphs();
}


void GraphStore::deleteAllGraphs()
{
	auto iterator = graphs.getBeginConstIterator();

	while (!iterator.isFinished())
	{
		delete iterator.getCurrent();
		iterator.goToNext();
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
	auto iterator = graphs.getBeginConstIterator();

	while (!iterator.isFinished())
	{
		const Graph* graph = iterator.getCurrent();
		
		if (graph->getID() == ID)
		{
			return graph;
		}

		iterator.goToNext();
	}

	return nullptr;
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
	size_t count = graphs.getCount();

	for (size_t i = 0; i < count; ++i)
	{
		if (graphs[i]->getID() == ID)
		{
			removeGraphAt(i);
			return;
		}
	}

	throwNonExistingGraph(ID);
}


void GraphStore::removeGraphAt(size_t index)
{
	Graph* toRemove = graphs[index];
	size_t lastGraphIndex = graphs.getCount() - 1;

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