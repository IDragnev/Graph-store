#ifndef __VERTEX_H_INCLUDED__
#define __VERTEX_H_INCLUDED__

#include "../../Special Integer/specialInteger.h"
#include "../../String/String.h"
#include "../../Singly Linked List/SinglyLinkedList.h"
#include "../Edge/Edge.h"

typedef specialInteger<unsigned> Distance; 

class Vertex
{
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
	Vertex(String ID, size_t index);
	~Vertex() = default;

	Vertex(const Vertex& source) = delete;
	Vertex& operator=(const Vertex& rhs) = delete;

	void setID(String ID);

private:
	size_t index;
	String id;
	SinglyLinkedList<Edge> edges;
	Vertex* parentInAlgorithmTree;
	Distance distanceToSource;
	bool visited;
};

bool operator==(const Vertex& lhs, const Vertex& rhs);
bool operator!=(const Vertex& lhs, const Vertex& rhs);

#endif // __VERTEX_H_INCLUDED__
