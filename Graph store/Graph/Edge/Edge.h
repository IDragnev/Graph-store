#ifndef __VERTEX_EDGE_H_INCLUDED__
#define __VERTEX_EDGE_H_INCLUDED__

class Vertex;

class Edge
{
	friend class Graph;
public:
	typedef unsigned Weight;

	Vertex& getIncidentVertex();
	const Vertex& getIncidentVertex() const;
	Weight getWeight() const;

private:
	Edge(Vertex* incidentVertex, Weight weight = 1);
	Edge(const Edge& source) = default;
	Edge(Edge&& source) = default;
	Edge& operator=(const Edge& rhs) = default;
	Edge& operator=(Edge&& rhs) = default;

private:
	Weight weight;
	Vertex* incidentVertex;
};

#endif // __VERTEX_EDGE_H_INCLUDED__
