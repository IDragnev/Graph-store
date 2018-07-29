#ifndef __VERTEX_H_INCLUDED__
#define __VERTEX_H_INCLUDED__

#include "../../Special Integer/SpecialInteger.h"
#include "../../String/String.h"
#include "../../Singly Linked List/SinglyLinkedList.h"
#include "../Edge/Edge.h"
#include "../../Priority Queue/Priority Queue Handle/PriorityQueueHandle.h"

typedef SpecialInteger<unsigned> Distance; 

class Vertex
{
	friend class Graph;
public:
	bool isVisited() const;
	void markAsVisited();
	void markAsNotVisited();

	Vertex* getPredecessor();
	const Vertex* getPredecessor() const;
	void setPredecessor(Vertex* predecessor);

	const Distance& getDistanceToSource() const;
	void setDistanceToSource(const Distance& distance);

	const PriorityQueueHandle& getPriorityQueueHandle() const;
	void setPriorityQueueHandle(const PriorityQueueHandle& handle);

	const String& getID() const;

private:
	Vertex(String ID, size_t index);

	Vertex(Vertex&& source) = delete;
	Vertex(const Vertex& source) = delete;
	Vertex& operator=(Vertex&& rhs) = delete;
	Vertex& operator=(const Vertex& rhs) = delete;

	void setID(String ID);

private:
	size_t index;
	String id;
	SinglyLinkedList<Edge> edges;
	Vertex* predecessor;
	PriorityQueueHandle handle;
	Distance distanceToSource;
	bool visited;
};

bool operator==(const Vertex& lhs, const Vertex& rhs);
bool operator!=(const Vertex& lhs, const Vertex& rhs);

#endif // __VERTEX_H_INCLUDED__
