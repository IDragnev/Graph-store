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
			explicit GraphCreator(String graphType) :
				createdGraphType{ std::move(graphType) }
			{
			}
			GraphCreator(const GraphCreator&) = delete;
			GraphCreator& operator=(const GraphCreator&) = delete;
			virtual ~GraphCreator() = default;

			virtual GraphPtr createEmptyGraph(String ID) const = 0;

			const String& getCreatedGraphType() const noexcept { return createdGraphType; }

		private:
			const String createdGraphType;
		};
	}
}

#endif //__GRAPH_CREATOR_H_INCLUDED__