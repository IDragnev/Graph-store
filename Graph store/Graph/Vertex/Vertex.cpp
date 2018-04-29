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


void Vertex::setID(String ID)
{
	if (ID != String(""))
	{
		this->id = std::move(ID);
	}
	else
	{
		throw std::invalid_argument("Vertex ID must be a valid string");
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


typename Vertex::EdgeIterator* Vertex::getEdgeIterator()
{
	return edges.getHeadIterator();
}


Vertex* Vertex::getParentInAlgorithmTree()
{
	return parentInAlgorithmTree;
}


const Vertex* Vertex::getParentInAlgorithmTree() const
{
	return parentInAlgorithmTree;
}


void Vertex::setParentInAlgorithmTree(Vertex* vertex)
{
	parentInAlgorithmTree = vertex;
}


const Distance& Vertex::getDistanceToSource() const
{
	return distanceToSource;
}


void Vertex::setDistanceToSource(const Distance& distance)
{
	distanceToSource = distance;
}