#include "GraphFactory.h"
#include "..\Graph\Base Graph\Graph.h"
#include "Graph creators\Base\GraphCreator.h"
#include "..\General Exceptions\Exception.h"
#include "..\String\String.h"
#include <algorithm>


GraphFactory::GraphFactory() :
	creators{ EXPECTED_CREATORS_COUNT }
{
}


GraphFactory& GraphFactory::instance()
{
	static GraphFactory theOnlyInstance;

	return theOnlyInstance;
}


GraphFactory::GraphPtr GraphFactory::createEmptyGraph(const String& type, const String& ID) const
{
	auto& creator = getCreator(type);

	return creator.createEmptyGraph(ID);
}


const GraphCreator& GraphFactory::getCreator(const String& graphType) const
{
	auto* creator = searchCreator(graphType);

	if (creator)
	{
		return *creator;
	}
	else
	{
		throw Exception{ "Invalid graph type" };
	}
}


const GraphCreator* GraphFactory::searchCreator(const String& graphType) const
{
	for (auto&& c : creators)
	{
		if (c->getCreatedGraphType() == graphType)
		{
			return c;
		}
	}

	return nullptr;
}


void GraphFactory::registerCreator(const GraphCreator* creator)
{
	assert(searchCreator(creator->getCreatedGraphType()) == nullptr);

	creators.insert(creator);
}






