#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\String\String.h"
#include "..\..\Special Integer\specialInteger.h"
#include "..\..\Singly Linked List\SinglyLinkedList.h"

class ShortestPathAlgorithm
{
protected:
	using Edge = Graph::Edge;
	using Vertex = Graph::Vertex;
	using EdgeConstIteratorPtr = std::unique_ptr<ConstIterator<Edge>>;

public:
	class Path
	{
	private:
		using Collection = SinglyLinkedList<String>;
		using Length = SpecialInteger<unsigned>;

	public:
		Path() = default;
		Path(Path&& source);
		Path(const Vertex* end);
		Path(const Path& source) = default;
	
		Path& operator=(Path&& rhs);
		Path& operator=(const Path& rhs) = default;

		void print() const;
		const Length& getLength() const;

	private:
		Collection IDs;
		Length len = Length::Infinity();
	};

public:
	explicit ShortestPathAlgorithm(const char* ID);
	virtual ~ShortestPathAlgorithm() = default;

	virtual Path findShortestPath(Graph& graph, const Vertex& source, const Vertex& goal) = 0;

	const String& getID() const;

protected:
	bool isTheGoal(const Vertex& v) const;	
	EdgeConstIteratorPtr getEdgesLeaving(const Vertex& v) const;

	void initBase(Graph& graph, const Vertex& goal);

private:
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = delete;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = delete;

private:
	const String id;
	Graph* searchedGraph;
	const Vertex* goal;
};

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
