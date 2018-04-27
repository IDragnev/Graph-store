#ifndef __VERTEX_EDGE_H_INCLUDED__
#define __VERTEX_EDGE_H_INCLUDED__

class Vertex;

class Edge
{
public:
	typedef unsigned Weight;

	Edge(Vertex* adjacentVertex, Weight weight = 1);

	Vertex* getAdjacentVertex();
	Weight getWeight() const;

	void setAdjacentVertex(Vertex* vertex);
	void setWeight(Weight weight);

private:
	Weight weight;
	Vertex* adjacentVertex;
};

#endif // __VERTEX_EDGE_H_INCLUDED__
