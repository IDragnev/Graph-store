#include "Graph.h"


Graph::Graph(String ID) :
	id(),
	vertices(LEAST_VERTICES_EXPECTED),
	verticesSearchTable(LEAST_VERTICES_EXPECTED)
{
	setID(ID);
}


Graph::~Graph()
{
	VertexIterator verticesIterator = vertices.getHeadIterator();

	while (verticesIterator)
	{
		deleteVertex(*verticesIterator);

		++verticesIterator;
	}
}