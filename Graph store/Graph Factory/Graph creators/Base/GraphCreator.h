#ifndef __GRAPH_CREATOR_H_INCLUDED__
#define __GRAPH_CREATOR_H_INCLUDED__

#include "..\..\..\String\String.h"
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		class Graph;

		class GraphCreator
		{
		protected:
			using GraphPtr = std::unique_ptr<Graph>;

		public:
			explicit GraphCreator(const String& graphType);
			GraphCreator(const GraphCreator&) = delete;
			GraphCreator& operator=(const GraphCreator&) = delete;
			virtual ~GraphCreator() = default;

			virtual GraphPtr createEmptyGraph(const String& ID) const = 0;

			const String& getCreatedGraphType() const;

		private:
			const String createdGraphType;
		};
	}
}

#endif //__GRAPH_CREATOR_H_INCLUDED__