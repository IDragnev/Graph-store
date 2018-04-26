#ifndef __VERTEX_H_INCLUDED__
#define __VERTEX_H_INCLUDED__

#include "../../Special Integer/specialInteger.h"
#include "../../String/String.h"
#include "../../Singly Linked List/SinglyLinkedList.h"

typedef specialInteger<unsigned> Distance;
class Edge;

class Vertex
{
	typedef SinglyLinkedList<Edge> AdjacencyList;
	typedef Iterator<Edge> EdgesIterator;
	friend class Graph;

private:
	Vertex(size_t index, String ID);

public:
	const String& getID() const;
	EdgesIterator* getEdgesIterator();

	bool isVisited() const;
	void markVisited();
	void markNotVisited();

	Vertex* getParentInAlgorithmTree();  // TODO: const Vertex* and const-qualified function?
	void setParentInAlgorithmTree(Vertex* parent);

	const Distance& getDistance() const;
	void setDistance(const Distance& distance);

private:
	size_t index;
	String id;
	AdjacencyList edges;
	Distance distanceToSource;
	Vertex* parentInAlgorithmTree;
	bool isVisited;
};

#endif // __VERTEX_H_INCLUDED__
