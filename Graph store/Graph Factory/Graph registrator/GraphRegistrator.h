#ifndef __GRAPH_REGISTRATOR_H_INCLUDED__
#define __GRAPH_REGISTRATOR_H_INCLUDED__

#include "Graph Factory\Graph creators\Concrete creator\ConcreteGraphCreator.h"
#include "Graph Factory\GraphFactory.h"

namespace IDragnev::GraphStore
{
	template <typename GraphType>
	class GraphRegistrator
	{
	private:
		using Creator = ConcreteGraphCreator<GraphType>;

	public:
		explicit GraphRegistrator(String graphType) :
			creator{ std::move(graphType) }
		{
			GraphFactory::instance().registerCreator(&creator);
		}

		GraphRegistrator(const GraphRegistrator&) = delete;
		GraphRegistrator& operator=(const GraphRegistrator&) = delete;

	private:
		const Creator creator;
	};
}

#endif //__GRAPH_REGISTRATOR_H_INCLUDED__