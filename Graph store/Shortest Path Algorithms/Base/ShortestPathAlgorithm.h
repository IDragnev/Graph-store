#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "..\..\String\String.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\Special Integer\specialInteger.h"
#include "..\..\Singly Linked List\SinglyLinkedList.h"

class ShortestPathAlgorithm
{
protected:
	using Edge = Graph::Edge;
	using Vertex = Graph::Vertex;
	using Distance = SpecialInteger<unsigned>;
	using EdgeConstIteratorPtr = std::unique_ptr<ConstIterator<Edge>>;

	struct VertexWrapper
	{
		VertexWrapper() = default;
		VertexWrapper(const Vertex* v) : wrappedVertex{ v } {}

		const Vertex* wrappedVertex = nullptr;
		const VertexWrapper* predecessor = nullptr;
		Distance distanceToSource = Distance::Infinity();
	};

public:
	class Path
	{
	private:
		using Collection = SinglyLinkedList<String>;

	public:
		Path() = default;
		Path(Path&& source);
		Path(const Path& source) = default;
		Path(const VertexWrapper& last);
	
		Path& operator=(Path&& rhs);
		Path& operator=(const Path& rhs) = default;

		void print() const;
		const Distance& getLength() const;

	private:
		Collection IDs;
		Distance length = Distance::Infinity();
	};

public:
	explicit ShortestPathAlgorithm(const char* ID);
	virtual ~ShortestPathAlgorithm() = default;

	Path findShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal);

	const String& getID() const;

protected:
	bool isTheGoal(const Vertex& v) const;	
	EdgeConstIteratorPtr getEdgesLeaving(const Vertex& v) const;

private:
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = delete;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = delete;

	virtual Path findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal) = 0;
	static Path buildTrivialPath(const Vertex& v);

	void initBase(const Graph& graph, const Vertex& goal);

private:
	const String id;
	const Graph* searchedGraph;
	const Vertex* goal;
};

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
