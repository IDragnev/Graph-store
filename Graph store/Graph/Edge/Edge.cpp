#include "Edge.h"
#include "..\Vertex\Vertex.h"
#include <assert.h>


Edge::Edge(Vertex* incidentVertex, unsigned weight) :
	weight{ weight },
	incidentVertex{ incidentVertex }
{
	assert(incidentVertex);
}


Vertex& Edge::getIncidentVertex() 
{
	return const_cast<Vertex&>( static_cast<const Edge&>(*this).getIncidentVertex() );
}


const Vertex& Edge::getIncidentVertex() const
{
	assert(incidentVertex);
	return *incidentVertex;
}


unsigned Edge::getWeight() const
{
	return weight;
}


