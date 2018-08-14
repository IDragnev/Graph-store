#ifndef __GRAPH_STORE_H_INCLUDED__
#define __GRAPH_STORE_H_INCLUDED__

#include "..\Graph\Base Graph\Graph.h"
#include "..\Dynamic Array\DArray.h"
#include "..\Exception\Exception.h"

class GraphStore;
typedef Exception<GraphStore> GraphStoreException;

class GraphStore
{
private:
	typedef DArray<Graph*> GraphCollection;

public:
	GraphStore() = default;
	GraphStore(GraphStore&& source) = default;
	~GraphStore();

	GraphStore& operator=(GraphStore&& rhs);

	void insertGraph(Graph& graph);
	void removeGraph(const String& ID);
	Graph& getGraph(const String& ID);
	const Graph& getGraph(const String& ID) const;
	
	bool isEmpty() const;
	void empty();

private:
	GraphStore(const GraphStore&) = delete;
	GraphStore& operator=(const GraphStore&) = delete;

	bool hasGraphWithID(const String& ID) const;
	const Graph* searchGraph(const String& ID) const;

	void removeGraphAt(size_t index);
	void deleteAllGraphs();

	static void throwNonExistingGraph(const String& ID);

private:
	GraphCollection graphs;
};

#endif //__GRAPH_STORE_H_INCLUDED__