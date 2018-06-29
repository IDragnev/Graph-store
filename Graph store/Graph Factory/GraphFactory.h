#ifndef __GRAPH_FACTORY_H_INCLUDED__
#define __GRAPH_FACTORY_H_INCLUDED__

#include "..\Graph\Base Graph\Graph.h"

class GraphCreator;

template <typename T>
class DArray;

class GraphFactory
{
private:
	template <typename GraphType>
	friend class GraphRegistrator;

	typedef DArray<const GraphCreator*> CreatorsCollection;
	typedef std::unique_ptr<Graph> GraphPtr;

public:
	static GraphPtr createEmptyGraph(const char* graphType, const char* graphID);

private:
	static const GraphCreator& getCreator(const char* graphType);
	static void registerCreator(const GraphCreator* creator);

private:
	static const GraphCreator* searchCreator(const String& graphType);
	static CreatorsCollection& getCreatorsCollection();

	static const size_t EXPECTED_CREATORS_COUNT = 2;
};

#endif //__GRAPH_FACTORY_H_INCLUDED__