#include "Edge.h"
#include "../Vertex/Vertex.h"


Edge::Edge(Vertex* incidentVertex, Weight weight) :
	weight(weight),
	incidentVertex(nullptr)
{
	setIncidentVertex(incidentVertex);
}


Vertex* Edge::getIncidentVertex() 
{
	return incidentVertex;
}


Edge::Weight Edge::getWeight() const
{
	return weight;
}


void Edge::setWeight(Weight newWeight)
{
	weight = newWeight;
}


void Edge::setIncidentVertex(Vertex* vertex)
{
	if (vertex != nullptr)
	{
		incidentVertex = vertex;
	}
	else
	{
		throw std::invalid_argument("Cannot set incident vertex to nullptr");
	}
}

