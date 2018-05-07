#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

//members
#include "../Hash/Hash.h"
#include "../Dynamic Array/DArray.h"
#include "../String/String.h"

//nested types
#include "Vertex\Vertex.h"
#include "Edge\Edge.h"


class Graph
{
private:
	class KeyAccessor; //TODO: remove this declaration afterwards

	typedef Hash<Vertex*, String, KeyAccessor> VertexHashTable;
	typedef DArray<Vertex*> VertexArray;
	typedef DArray<Vertex*>::DArrayIterator<Vertex*> VertexIterator;

protected:
	typedef Iterator<Vertex*> AbstractVertexIterator;
	typedef Iterator<Edge> AbstractEdgeIterator;

public:
	/*
	.
	.
	.
	.
	.
	*/

private:
	String id;
	VertexArray vertices;
	VertexHashTable verticesSearchTable;
};

#endif // __BASE_GRAPH_H_INCLUDED__