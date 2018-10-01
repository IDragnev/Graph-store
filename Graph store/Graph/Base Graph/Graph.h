#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "..\..\Hash\Hash.h"
#include "..\..\Hash\HashFunctionStringSpecialization.h"
#include "..\..\Dynamic Array\DArray.h"
#include "..\..\String\String.h"
#include "..\..\Singly Linked List\SinglyLinkedList.h"
#include "..\..\Iterator abstraction\Iterator.h"
#include <memory>

class Graph
{
public:
	class Vertex;
	class Edge
	{
	public:
		Edge(Vertex* incidentVertex, unsigned weight = 1);

		Vertex& getIncidentVertex();
		const Vertex& getIncidentVertex() const;
		unsigned getWeight() const;

	private:
		Vertex* incidentVertex;
		unsigned weight;
	};

	class Vertex
	{
		friend class Graph;
	public:
		Vertex(const String& ID, std::size_t index);

		const String& getID() const;

	private:
		Vertex(const Vertex&) = delete;
		Vertex& operator=(const Vertex&) = delete;

		void setID(const String& ID);

	private:
		String id;
		std::size_t index;
		SinglyLinkedList<Edge> edges;
	};

private:
	struct IDAccessor
	{
		template <typename T>
		const String& operator()(const T& item) const
		{
			return item.getID();
		}
	};

	using VertexHashTable = Hash<Vertex, String, IDAccessor>;
	using EdgeIterator = SinglyLinkedList<Edge>::iterator;
	using VertexConstIteratorPtr = std::unique_ptr<ConstIterator<Vertex*>>;
	using EdgeIteratorPtr = std::unique_ptr<Iterator<Edge>>;
	using EdgeConstIteratorPtr = std::unique_ptr<ConstIterator<Edge>>;
	
public:
	Graph(const String& ID);
	virtual ~Graph();

	void insertVertexWithID(const String& ID);
	void removeVertex(const String& ID);
	void removeVertex(Vertex& vertex);

	virtual void insertEdge(Vertex& start, Vertex& end, unsigned weight = 1) = 0;
	virtual void removeEdge(Vertex& start, Vertex& end) = 0;

	Vertex& getVertex(const String& ID);
	const Vertex& getVertex(const String& ID) const;

	unsigned getVerticesCount() const;
	VertexConstIteratorPtr getIteratorToVertices() const;
	EdgeIteratorPtr getIteratorToEdgesLeaving(Vertex& vertex);
	EdgeConstIteratorPtr getConstIteratorToEdgesLeaving(const Vertex& vertex) const;

	const String& getID() const;

protected:
	virtual void removeEdgesEndingIn(Vertex& vertex) = 0;
	static void removeEdgeFromToNoThrow(Vertex& from, Vertex& to);
	static void removeEdgeFromTo(Vertex& from, Vertex& to);
	static void insertEdgeFromToWithWeight(Vertex& from, Vertex& to, unsigned weight);
	static bool existsEdgeFromTo(Vertex& from, Vertex& to);

	bool hasVertexWithID(const String& ID) const;
	bool isOwnerOf(const Vertex& vertex) const;

private:
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;

	static void removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist);
	static EdgeIterator searchEdgeFromTo(Vertex& from, Vertex& to);

	void tryToInsertVertexWithID(const String& ID);
	void insert(Vertex& vertex);
	void insertInVertices(Vertex& vertex);
	void insertInSearchTable(Vertex& vertex);
	void removeFromVertices(const Vertex& vertex);
	void removeFromSearchTable(const Vertex& vertex);

	std::unique_ptr<Vertex> createVertex(const String& ID) const;
	static void deleteVertex(Vertex* vertex);

	void setID(const String& ID);

private:
	static const std::size_t FEWEST_VERTICES_EXPECTED = 32;

private:
	String id;
	DArray<Vertex*> vertices;
	VertexHashTable verticesSearchTable;
};

bool operator==(const Graph::Vertex& lhs, const Graph::Vertex& rhs);
bool operator!=(const Graph::Vertex& lhs, const Graph::Vertex& rhs);

#endif // __BASE_GRAPH_H_INCLUDED__