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
	Vertex(String ID, size_t index);

public:
	const String& getID() const;
	EdgesIterator* getEdgesIterator();

	bool isVisited() const;
	void markAsVisited();
	void markAsNotVisited();

	Vertex* getParentInAlgorithmTree();
	void setParentInAlgorithmTree(Vertex* parent);

	const Distance& getDistance() const;
	void setDistance(const Distance& distance);

private:
	size_t index;
	String id;
	AdjacencyList edges;
	Vertex* parentInAlgorithmTree;
	Distance distanceToSource;
	bool isVisited;
};

#endif // __VERTEX_H_INCLUDED__
