#include "Vertex.h"
#include "../Graph Exception/GraphException.h"


Vertex::Vertex(const String& ID, size_t index) :
	index(index),
	id(),
	edges(),
	predecessor(nullptr),
	handle(),
	distanceToSource(Distance::Infinity()),
	visited(false)
{
	setID(ID);
}


void Vertex::setID(const String& ID)
{
	if (ID != String(""))
	{
		this->id = ID;
	}
	else
	{
		throw GraphException("Vertex ID must be a valid string");
	}
}


const String& Vertex::getID() const
{
	return id;
}


bool Vertex::isVisited() const
{
	return visited;
}


void Vertex::markAsNotVisited()
{
	visited = false;
}


void Vertex::markAsVisited()
{
	visited = true;
}


Vertex* Vertex::getPredecessor()
{
	return const_cast<Vertex*>( static_cast<const Vertex&>(*this).getPredecessor() );
}


const Vertex* Vertex::getPredecessor() const
{
	return predecessor;
}


void Vertex::setPredecessor(Vertex* vertex)
{
	predecessor = vertex;
}


const Distance& Vertex::getDistanceToSource() const
{
	return distanceToSource;
}


void Vertex::setDistanceToSource(const Distance& distance)
{
	distanceToSource = distance;
}


const PriorityQueueHandle& Vertex::getPriorityQueueHandle() const
{
	return handle;
}


void Vertex::setPriorityQueueHandle(const PriorityQueueHandle& handle)
{
	this->handle = handle;
}


bool operator==(const Vertex& lhs, const Vertex& rhs)
{
	//vertices are equal if and only if 
	//they are the same object
	return &lhs == &rhs;
}


bool operator!=(const Vertex& lhs, const Vertex& rhs)
{
	return !(lhs == rhs);
}