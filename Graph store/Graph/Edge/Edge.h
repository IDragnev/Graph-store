#ifndef __VERTEX_EDGE_H_INCLUDED__
#define __VERTEX_EDGE_H_INCLUDED__

class Vertex;

class Edge
{
	friend class Graph;
public:
	typedef unsigned Weight;

	Vertex* getIncidentVertex();
	const Vertex* getIncidentVertex() const;
	Weight getWeight() const;

private:
	Edge(Vertex* incidentVertex, Weight weight = 1);

private:
	Weight weight;
	Vertex* incidentVertex;
};

#endif // __VERTEX_EDGE_H_INCLUDED__
