#ifndef __GRAPH_FACTORY_H_INCLUDED__
#define __GRAPH_FACTORY_H_INCLUDED__

#include "..\Dynamic Array\DArray.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\String\String.h"

class GraphCreator;

class GraphFactory
{
private:
	template <typename GraphType>
	friend class GraphRegistrator;

	typedef DArray<const GraphCreator*> CreatorsCollection;
	typedef std::unique_ptr<Graph> GraphPtr;

public:
	static GraphFactory& instance();
	
	GraphPtr createEmptyGraph(const String& type, const String& ID) const;

private:
	GraphFactory();
	~GraphFactory() = default;

	GraphFactory(GraphFactory&&) = delete;
	GraphFactory(const GraphFactory&) = delete;
	GraphFactory& operator=(GraphFactory&&) = delete;
	GraphFactory& operator=(const GraphFactory&) = delete;

private:
	const GraphCreator& getCreator(const String& graphType) const;
	const GraphCreator* searchCreator(const String& graphType) const;
	void registerCreator(const GraphCreator* creator);

	static const size_t EXPECTED_CREATORS_COUNT = 2;

private:
	CreatorsCollection creators;
};

#endif //__GRAPH_FACTORY_H_INCLUDED__