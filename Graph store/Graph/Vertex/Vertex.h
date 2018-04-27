#ifndef __VERTEX_H_INCLUDED__
#define __VERTEX_H_INCLUDED__

#include "../../Special Integer/specialInteger.h"
#include "../../String/String.h"
#include "../../Singly Linked List/SinglyLinkedList.h"
#include "../Edge/Edge.h"

typedef specialInteger<unsigned> Distance;

class Vertex
{
private:
	typedef size_t Handle;
	typedef SinglyLinkedList<Edge> EdgeList;
	typedef Iterator<Edge> EdgeIterator;

	friend class Graph;

private:
	Vertex(String ID, Handle index);

public:
	bool isVisited() const;
	void markAsVisited();
	void markAsNotVisited();

	const String& getID() const;
	EdgeIterator* getEdgeIterator();

	Vertex* getParentInAlgorithmTree();
	void setParentInAlgorithmTree(Vertex* parent);

	const Distance& getDistanceToSource() const;
	void setDistanceToSource(const Distance& distance);

private:
	void setID(String newID);

private:
	Handle index;
	String id;
	EdgeList edges;
	Vertex* parentInAlgorithmTree;
	Distance distanceToSource;
	bool visited;
};

#endif // __VERTEX_H_INCLUDED__
