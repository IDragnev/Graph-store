#include "GraphFactory.h"
#include "Graph creators\Base\GraphCreator.h"
#include "..\Graph\Graph Exception\GraphException.h"
#include "..\Dynamic Array\DArray.h"


GraphFactory::GraphPtr GraphFactory::createEmptyGraph(const String& graphType, const String& graphID)
{
	const GraphCreator& creator = getCreator(graphType);

	return creator.createEmptyGraph(graphID);
}


const GraphCreator& GraphFactory::getCreator(const String& graphType)
{
	const GraphCreator* creator = searchCreator(graphType);

	if (creator)
	{
		return *creator;
	}
	else
	{
		throw GraphException("Invalid graph type");
	}
}


const GraphCreator* GraphFactory::searchCreator(const String& graphType)
{
	CreatorsCollection& creators = getCreatorsCollection();
	auto iterator = creators.getHeadIterator();

	while (iterator)
	{
		const GraphCreator* creator = *iterator;

		if (creator->getCreatedGraphType() == graphType)
		{
			return creator;
		}

		++iterator;
	}

	return nullptr;
}


void GraphFactory::registerCreator(const GraphCreator* creator)
{
	assert(!searchCreator(creator->getCreatedGraphType()));

	CreatorsCollection& creators = getCreatorsCollection();
	creators.insert(creator);
}


GraphFactory::CreatorsCollection& GraphFactory::getCreatorsCollection()
{
	static CreatorsCollection graphCreators(EXPECTED_CREATORS_COUNT);

	return graphCreators;
}






