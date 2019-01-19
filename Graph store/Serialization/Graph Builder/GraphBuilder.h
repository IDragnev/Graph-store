#ifndef __GRAPH_BUILDER_H_INCLUDED__
#define __GRAPH_BUILDER_H_INCLUDED__

#include "..\..\String\String.h"
#include "..\..\File parser\FileParser.h"
#include "..\..\Containers\Dynamic Array\DArray.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		class Exception;

		class GraphBuilder
		{
		private:
			using GraphPtr = std::unique_ptr<Graph>;
			using StringArray = Containers::DArray<String>;
			using EdgeWeight = Graph::Edge::Weight;

			struct RawEdge
			{
				std::size_t startVertexIDIndex{};
				std::size_t endVertexIDIndex{};
				EdgeWeight weight{};
			};

		public:
			GraphBuilder() = default;
			~GraphBuilder() = default;

			GraphPtr buildFromFile(const String& filename);

		private:
			GraphBuilder(const GraphBuilder&) = delete;;
			GraphBuilder& operator=(const GraphBuilder&) = delete;

			void init(const String& filename);

			void build();
			void createEmptyGraph();
			void insertVertices();
			void parseVertexIDs();
			void insertEdges();
			void insertSingleEdge(const RawEdge& edge);
			RawEdge parseSingleEdge();
			std::uint32_t parseUnsignedAndIgnoreUntil(char symbol);
			Graph::Vertex& getVertex(std::size_t idIndex);

			void clean();

			void handleError(const String& filename, const Exception& e);
			bool areVerticesInserted() const;

		private:
			StringArray vertexIDs;
			GraphPtr result;
			FileParser parser;
		};
	}
}

#endif //__GRAPH_BUILDER_H_INCLUDED__