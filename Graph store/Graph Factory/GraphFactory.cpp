#include "GraphFactory.h"
#include "..\Graph\Base Graph\Graph.h"
#include "Graph creators\Base\GraphCreator.h"
#include "..\General Exceptions\Exception.h"
#include "..\String\String.h"
#include "..\UtilityFunctions.h"
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
			using GraphType = Utility::ConstStringIDRef;
			using Functional::matches;
			
			auto extractor = [](const auto& creator) -> decltype(auto) { return creator->getCreatedGraphType(); };
			return matches(GraphType(graphType), extractor);
		}

		auto GraphFactory::searchCreator(const String& graphType) const -> CreatorsCollection::const_iterator
		{
			return std::find_if(creators.cbegin(), creators.cend(), matches(graphType));
		}

		void GraphFactory::registerCreator(const GraphCreator* creator)
		{
			assert(!searchCreator(creator->getCreatedGraphType()));
			creators.insert(creator);
		}
	}
}





