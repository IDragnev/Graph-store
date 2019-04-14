#include "GraphFactory.h"
#include "..\Graph\Base Graph\Graph.h"
#include "Graph creators\Base\GraphCreator.h"
#include "..\General Exceptions\Exception.h"
#include "..\String\String.h"
#include "..\..\Third party\NamedType\named_type.hpp"
#include <algorithm>

namespace IDragnev
{
	namespace GraphStore
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

		GraphFactory::GraphPtr GraphFactory::createEmptyGraph(const String& type, const String& ID) const
		{
			auto& creator = getCreator(type);
			return creator.createEmptyGraph(ID);
		}

		const GraphCreator& GraphFactory::getCreator(const String& graphType) const
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

		auto matches(const String& graphType)
		{
			using Functional::matches;
			using namespace fluent;
			using GraphTypeRef = NamedType<const String&, struct GraphTypeTag, Comparable>;
			
			auto extractor = [](const auto& creator) { return GraphTypeRef(creator->getCreatedGraphType()); };
			return matches(GraphTypeRef(graphType), extractor);
		}

		auto GraphFactory::searchCreator(const String& graphType) const -> CreatorsCollection::const_iterator
		{
			return std::find_if(std::begin(creators), std::end(creators), matches(graphType));
		}

		void GraphFactory::registerCreator(const GraphCreator* creator)
		{
			assert(!searchCreator(creator->getCreatedGraphType()));
			creators.insertBack(creator);
		}
	}
}





