#include "Edge.h"
#include "../Vertex/Vertex.h"
#include <assert.h>


Edge::Edge(Vertex* incidentVertex, Weight weight) :
	weight(weight),
	incidentVertex(incidentVertex)
{
	assert(this->incidentVertex);
}


Vertex& Edge::getIncidentVertex() 
{
	assert(incidentVertex);

	return *incidentVertex;
}


const Vertex& Edge::getIncidentVertex() const
{
	assert(incidentVertex);

	return *incidentVertex;
}


Edge::Weight Edge::getWeight() const
{
	return weight;
}


