#ifndef __VERTEX_EDGE_H_INCLUDED__
#define __VERTEX_EDGE_H_INCLUDED__

class Vertex;

class Edge
{
public:
	Edge(Vertex* incidentVertex, unsigned weight = 1);

	Vertex& getIncidentVertex();
	const Vertex& getIncidentVertex() const;
	unsigned getWeight() const;

private:
	unsigned weight;
	Vertex* incidentVertex;
};

#endif // __VERTEX_EDGE_H_INCLUDED__
