#include "Edge.h"
#include "../Vertex/Vertex.h"


Edge::Edge(Vertex* adjacentVertex, Weight weight) :
	weight(weight),
	adjacentVertex(nullptr)
{
	setAdjacentVertex(adjacentVertex);
}


Vertex* Edge::getAdjacentVertex() 
{
	return adjacentVertex;
}


Edge::Weight Edge::getWeight() const
{
	return weight;
}


void Edge::setWeight(Weight newWeight)
{
	weight = newWeight;
}


void Edge::setAdjacentVertex(Vertex* vertex)
{
	if (vertex != nullptr)
	{
		adjacentVertex = vertex;
	}
	else
	{
		throw std::invalid_argument("Cannot set adjacent vertex to nullptr");
	}
}

