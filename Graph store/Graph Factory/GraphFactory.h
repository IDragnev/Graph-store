#ifndef __GRAPH_FACTORY_H_INCLUDED__
#define __GRAPH_FACTORY_H_INCLUDED__

#include "Third party\NamedType\named_type.hpp"
#include "Containers\Dynamic Array\DArray.h"
#include <memory>

namespace IDragnev
{
	class String;
}

namespace IDragnev::GraphStore
{
	class Graph;
	class GraphCreator;

	class GraphFactory
	{
	private:
		template <typename GraphType>
		friend class GraphRegistrator;

		using CreatorsCollection = Containers::DArray<const GraphCreator*>;
		using GraphPtr = std::unique_ptr<Graph>;

	public:
		using GraphTypeRef = fluent::NamedType<const String&, struct GraphTypeTag, fluent::Comparable>;
		using GraphIDRef = fluent::NamedType<const String&, struct GraphIDTag>;

		static GraphFactory& instance();

		GraphPtr createEmptyGraph(GraphTypeRef, GraphIDRef) const;

	private:
		GraphFactory();
		~GraphFactory() = default;

		GraphFactory(const GraphFactory&) = delete;
		GraphFactory& operator=(const GraphFactory&) = delete;

	private:
		const GraphCreator& getCreator(GraphTypeRef) const;
		CreatorsCollection::const_iterator searchCreator(GraphTypeRef) const;
		void registerCreator(const GraphCreator* creator);

		static constexpr std::size_t EXPECTED_CREATORS_COUNT = 2;

	private:
		CreatorsCollection creators;
	};
}

#endif //__GRAPH_FACTORY_H_INCLUDED__