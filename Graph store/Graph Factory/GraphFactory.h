#ifndef __GRAPH_FACTORY_H_INCLUDED__
#define __GRAPH_FACTORY_H_INCLUDED__

#include "..\Containers\Dynamic Array\DArray.h"
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		class Graph;
		class GraphCreator;
		class String;

		class GraphFactory
		{
		private:
			template <typename GraphType>
			friend class GraphRegistrator;

			using CreatorsCollection = Containers::DArray<const GraphCreator*>;
			using GraphPtr = std::unique_ptr<Graph>;

		public:
			static GraphFactory& instance();

			GraphPtr createEmptyGraph(const String& type, const String& ID) const;

		private:
			GraphFactory();
			~GraphFactory() = default;

			GraphFactory(const GraphFactory&) = delete;
			GraphFactory& operator=(const GraphFactory&) = delete;

		private:
			const GraphCreator& getCreator(const String& graphType) const;
			const GraphCreator* searchCreator(const String& graphType) const;
			void registerCreator(const GraphCreator* creator);

			static const std::size_t EXPECTED_CREATORS_COUNT = 2;

		private:
			CreatorsCollection creators;
		};
	}
}
#endif //__GRAPH_FACTORY_H_INCLUDED__