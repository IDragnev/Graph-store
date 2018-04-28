#ifndef __VERTEX_EDGE_H_INCLUDED__
#define __VERTEX_EDGE_H_INCLUDED__

class Vertex;

class Edge
{
public:
	typedef unsigned Weight;

	Edge(Vertex* incidentVertex, Weight weight = 1);

	Vertex* getIncidentVertex();
	void setIncidentVertex(Vertex* vertex);

	Weight getWeight() const;
	void setWeight(Weight weight);

private:
	Weight weight;
	Vertex* incidentVertex;
};

#endif // __VERTEX_EDGE_H_INCLUDED__
