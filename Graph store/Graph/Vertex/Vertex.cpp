#include "Vertex.h"


Vertex::Vertex(String ID, Handle index) :
	index(index),
	id(),
	edges(),
	parentInAlgorithmTree(nullptr),
	distanceToSource(Distance::getInfinity()),
	visited(false)
{
	setID(std::move(ID));
}