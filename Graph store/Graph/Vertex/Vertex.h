#ifndef __VERTEX_H_INCLUDED__
#define __VERTEX_H_INCLUDED__

#include "../../Special Integer/specialInteger.h"
#include "../../String/String.h"
#include "../../Singly Linked List/SinglyLinkedList.h"
#include "../Edge/Edge.h"

typedef specialInteger<unsigned> Distance; //TODO: move this typedef to the Graph class ?

class Vertex
{
private:
	typedef size_t Handle;
	typedef SinglyLinkedList<Edge> EdgeList;
	typedef SinglyLinkedListIterator<Edge> EdgeIterator;

	friend class Graph;

public:
	bool isVisited() const;
	void markAsVisited();
	void markAsNotVisited();

	const String& getID() const;

	Vertex* getParentInAlgorithmTree();
	const Vertex* getParentInAlgorithmTree() const;
	void setParentInAlgorithmTree(Vertex* parent);

	const Distance& getDistanceToSource() const;
	void setDistanceToSource(const Distance& distance);

private:
	Vertex(String ID, Handle index);

	Vertex(const Vertex& source) = delete;
	Vertex& operator=(const Vertex& rhs) = delete;

	EdgeIterator getEdgeIterator();

	void setID(String ID);

private:
	Handle index;
	String id;
	EdgeList edges;
	Vertex* parentInAlgorithmTree;
	Distance distanceToSource;
	bool visited;
};

#endif // __VERTEX_H_INCLUDED__
