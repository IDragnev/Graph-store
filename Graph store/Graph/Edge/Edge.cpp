#include "Edge.h"
#include "../Vertex/Vertex.h"
#include <assert.h>


Edge::Edge(Vertex* incidentVertex, Weight weight) :
	weight(weight),
	incidentVertex(incidentVertex)
{
	assert(this->incidentVertex != nullptr);
}


Vertex* Edge::getIncidentVertex() 
{
	return incidentVertex;
}


const Vertex* Edge::getIncidentVertex() const
{
	return incidentVertex;
}


Edge::Weight Edge::getWeight() const
{
	return weight;
}


