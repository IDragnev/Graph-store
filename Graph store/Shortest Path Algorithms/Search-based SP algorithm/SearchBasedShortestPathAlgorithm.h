#ifndef __SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__
#define __SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__

#include "..\Base\ShortestPathAlgorithm.h"
#include "..\..\Hash\HashFunction.h"
#include "..\..\String\String.h"
#include <unordered_map>

class SearchBasedShortestPathAlgorithm : public ShortestPathAlgorithm
{
protected:
	struct MarkableVertex : VertexDecorator
	{
		using VertexDecorator::VertexDecorator;
		bool isVisited = false;
	};

	using VertexMap = std::unordered_map<String, MarkableVertex, HashFunction<String>>;

public:
	using ShortestPathAlgorithm::ShortestPathAlgorithm;

protected:
	void decorate(const Graph& graph, const Vertex& source);
	void cleanDecoratedState();

	MarkableVertex& decoratorOf(const Vertex& v);
	const MarkableVertex& decoratorOf(const Vertex& v) const;

private:
	virtual void initSourceDecorator(MarkableVertex& source) = 0;

private:
	VertexMap decorators{};
};

#endif //__SEARCH_BASED_SP_ALGORITHM_H_INCLUDED__