#include "GraphFactory.h"
#include "Graph\Base Graph\Graph.h"
#include "Graph creators\Base\GraphCreator.h"
#include "Exceptions\Exceptions.h"
#include "String\String.h"
#include <algorithm>

namespace IDragnev::GraphStore
{
	GraphFactory::GraphFactory() :
		creators{ EXPECTED_CREATORS_COUNT }
	{
	}

	GraphFactory& GraphFactory::instance()
	{
		static GraphFactory theOnlyInstance;

		return theOnlyInstance;
	}

	auto GraphFactory::createEmptyGraph(GraphTypeRef type, GraphIDRef ID) const -> GraphPtr
	{
		auto& creator = getCreator(type);

		return creator(ID.get());
	}

	const GraphCreator& GraphFactory::getCreator(GraphTypeRef graphType) const
	{
		if (auto iterator = searchCreator(graphType);
			iterator)
		{
			auto creatorPtr = *iterator;
			return *creatorPtr;
		}
		else
		{
			throw Exception{ "Invalid graph type" };
		}
	}

	auto GraphFactory::searchCreator(GraphTypeRef graphType) const -> CreatorsCollection::const_iterator
	{
		auto extractCreatedType = [](const auto& creator) { return GraphTypeRef(creator->getCreatedGraphType()); };
		auto matchesGraphType = Functional::matches(graphType, extractCreatedType);

		return std::find_if(std::begin(creators), std::end(creators), matchesGraphType);
	}

	void GraphFactory::registerCreator(const GraphCreator* creator)
	{
		assert(!searchCreator(GraphTypeRef(creator->getCreatedGraphType())));
		creators.insertBack(creator);
	}
}




