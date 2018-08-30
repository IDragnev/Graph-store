#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "..\Base\ShortestPathAlgorithm.h"
#include "..\..\Hash\HashFunction.h"
#include "..\..\Queue\Queue.h"
#include <unordered_map>

class BFSShortest: public ShortestPathAlgorithm
{
private:
	struct BFSVertex: public VertexWrapper
	{
		using VertexWrapper::VertexWrapper;
		bool isVisited = false;
	};

	using WrappersMap = std::unordered_map<String, BFSVertex, HashFunction<String>>;
	using VerticesQueue = Queue<const Vertex*>;

public:
	using ShortestPathAlgorithm::ShortestPathAlgorithm;

private:
	virtual Path findNonTrivialShortestPath(Graph& graph, const Vertex& source, const Vertex& goal) override;

	void wrap(Graph& graph, const Vertex& source);
	Path findShortestPathToGoalFrom(const Vertex& source);
	bool isFrontierEmpty() const;
	const Vertex& extractVertexFromTheFrontier();
	void expandFrontierFrom(const Vertex& vertex);
	void addToFrontier(const Vertex& vertex);
	static void extendCurrentPathFromTo(const BFSVertex& from, BFSVertex& to);

	void clean();

private:
	VerticesQueue queue{};
	WrappersMap wrappers{};
};

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__
