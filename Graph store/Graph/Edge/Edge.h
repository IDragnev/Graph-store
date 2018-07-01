#ifndef __VERTEX_EDGE_H_INCLUDED__
#define __VERTEX_EDGE_H_INCLUDED__

class Vertex;

class Edge
{
private:
	friend class Graph;
	typedef unsigned Weight;

public:
	Edge(Edge&& source) = default;
	Edge(const Edge& source) = default;

	Vertex& getIncidentVertex();
	const Vertex& getIncidentVertex() const;
	Weight getWeight() const;

private:
	Edge(Vertex* incidentVertex, Weight weight = 1);
	Edge& operator=(const Edge& rhs) = default;
	Edge& operator=(Edge&& rhs) = default;

private:
	Weight weight;
	Vertex* incidentVertex;
};

#endif // __VERTEX_EDGE_H_INCLUDED__
