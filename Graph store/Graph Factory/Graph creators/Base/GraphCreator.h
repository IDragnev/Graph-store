#ifndef __GRAPH_CREATOR_H_INCLUDED__
#define __GRAPH_CREATOR_H_INCLUDED__

#include "../../../String/String.h"
#include "../../../Graph/Base Graph/Graph.h"
#include <memory>

class GraphCreator
{
protected:
	typedef std::unique_ptr<Graph> GraphPtr;

public:
	explicit GraphCreator(const String& graphType);
	virtual ~GraphCreator() = default;

	virtual GraphPtr createEmptyGraph(const String& ID) const = 0;

	const String& getCreatedGraphType() const;

private:
	GraphCreator(GraphCreator&&) = delete;
	GraphCreator(const GraphCreator&) = delete;
	GraphCreator& operator=(GraphCreator&&) = delete;
	GraphCreator& operator=(const GraphCreator&) = delete;

private:
	const String createdGraphType;
};

#endif //__GRAPH_CREATOR_H_INCLUDED__