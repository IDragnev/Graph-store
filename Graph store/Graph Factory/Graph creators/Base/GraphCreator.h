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
	virtual ~GraphCreator() = default;

	virtual GraphPtr createEmptyGraph(const char* graphName) const = 0;

	const String& getCreatedGraphType() const;

protected:
	GraphCreator(const char* createdGraphType);

private:
	GraphCreator(GraphCreator&& source) = delete;
	GraphCreator(const GraphCreator& source) = delete;
	GraphCreator& operator=(const GraphCreator& rhs) = delete;
	GraphCreator& operator=(GraphCreator&& rhs) = delete;

private:
	const String createdGraphType;
};

#endif //__GRAPH_CREATOR_H_INCLUDED__