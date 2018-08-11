#include "GraphStore.h"


GraphStore::~GraphStore()
{
	auto iterator = graphs.getHeadConstIterator();

	while (!iterator.isFinished())
	{
		delete iterator.getCurrent();
		iterator.goToNext();
	}
}


void GraphStore::insert(Graph& graph)
{
	if (!hasGraphWithID(graph.getID()))
	{
		graphs.insert(&graph);
	}
	else
	{
		throw GraphStoreException("A graph with ID \'" + graph.getID() + String("\' already exists"));
	}
}


bool GraphStore::hasGraphWithID(const String& ID) const
{
	return searchGraph(ID) != nullptr;
}


const Graph* GraphStore::searchGraph(const String& ID) const
{
	auto iterator = graphs.getHeadConstIterator();

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


Graph& GraphStore::get(const String& ID)
{
	return const_cast<Graph&>( static_cast<const GraphStore&>(*this).get(ID) );
}


const Graph& GraphStore::get(const String& ID) const
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
	throw GraphStoreException("No graph with ID \'" + ID + String("\' exists"));
}


void GraphStore::remove(const String& ID)
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