#ifndef __GRAPH_STORE_H_INCLUDED__
#define __GRAPH_STORE_H_INCLUDED__

#include "..\Dynamic Array\DArray.h"

class String;
class Graph;
class Exception;

class GraphStore
{
private:
	using GraphCollection = DArray<Graph*>;

public:
	GraphStore() = default;
	GraphStore(const GraphStore&) = delete;
	GraphStore(GraphStore&& source) = default;
	~GraphStore();

	GraphStore& operator=(GraphStore&& rhs);
	GraphStore& operator=(const GraphStore&) = delete;

	void insertGraph(Graph& graph);
	void removeGraph(const String& ID);
	Graph& getGraph(const String& ID);
	const Graph& getGraph(const String& ID) const;
	
	bool isEmpty() const;
	void empty();

private:
	bool hasGraphWithID(const String& ID) const;
	const Graph* searchGraph(const String& ID) const;

	void removeGraphAt(std::size_t index);
	void deleteAllGraphs();

	static void throwNonExistingGraph(const String& ID);

private:
	GraphCollection graphs;
};

#endif //__GRAPH_STORE_H_INCLUDED__