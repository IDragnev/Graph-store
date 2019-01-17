#ifndef __CONCRETE_GRAPH_CREATOR__
#define __CONCRETE_GRAPH_CREATOR__

#include "..\Base\GraphCreator.h"
#include "..\..\..\Graph\Base Graph\Graph.h"

namespace IDragnev
{
	namespace GraphStore
	{
		template <typename GraphType>
		class ConcreteGraphCreator : public GraphCreator
		{
		public:
			using GraphCreator::GraphCreator;

			GraphPtr createEmptyGraph(String ID) const override
			{
				return std::make_unique<GraphType>(std::move(ID));
			}
		};
	}
}

#endif //__CONCRETE_GRAPH_CREATOR__