#ifndef __VERTEX_H_INCLUDED__
#define __VERTEX_H_INCLUDED__

#include "../../Special Integer/SpecialInteger.h"
#include "../../String/String.h"
#include "../../Singly Linked List/SinglyLinkedList.h"
#include "../Edge/Edge.h"
#include "../../Priority Queue/Priority Queue Handle/PriorityQueueHandle.h"

using Distance = SpecialInteger<unsigned>; 

class Vertex
{
	friend class Graph;
public:
	Vertex(const String& ID, std::size_t index);

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
	Vertex(const Vertex&) = delete;
	Vertex& operator=(const Vertex&) = delete;

	void setID(const String& ID);

private:
	std::size_t index;
	String id;
	SinglyLinkedList<Edge> edges;
	Vertex* predecessor;
	PriorityQueueHandle handle{};
	Distance distanceToSource{};
	bool visited;
};

bool operator==(const Vertex& lhs, const Vertex& rhs);
bool operator!=(const Vertex& lhs, const Vertex& rhs);

#endif // __VERTEX_H_INCLUDED__
