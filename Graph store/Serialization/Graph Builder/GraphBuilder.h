#ifndef __GRAPH_BUILDER_H_INCLUDED__
#define __GRAPH_BUILDER_H_INCLUDED__

#include "..\..\File parser\FileParser.h"
#include "..\..\Containers\Dynamic Array\DArray.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include <memory>

namespace IDragnev::GraphStore
{
	enum class EdgeFormat : char;

	class GraphBuilder
	{
	private:
		using GraphPtr = std::unique_ptr<Graph>;
		using StringArray = Containers::DArray<String>;
		using EdgeWeight = Graph::Edge::Weight;

		struct RawEdge
		{
			std::size_t startVertexIDIndex = 0;
			std::size_t endVertexIDIndex = 0;
			EdgeWeight weight = 0;
		};

	public:
		GraphBuilder() = default;
		GraphBuilder(const GraphBuilder&) = delete;

		GraphBuilder& operator=(const GraphBuilder&) = delete;

		GraphPtr operator()(const String& filename);

	private:
		void tryToBuildFrom(const String& filename);
		void init(const String& filename);
		void build();
		void createEmptyGraph();
		void insertVertices();
		void parseVertexIDs();
		void insertEdges();
		void insertSingleEdge(const RawEdge& edge);
		RawEdge parseSingleEdge();
		std::size_t parseUnsignedAndIgnoreUntil(char symbol);
		std::size_t parseUnsignedAndIgnoreUntil(EdgeFormat symbol);
		void ignoreUntil(EdgeFormat symbol);
		Graph::Vertex& getVertex(std::size_t idIndex);

		void clear();
		auto makeScopedClear() noexcept
		{
			auto deleter = [](auto ptr) { ptr->clear(); };
			using ScopedClear = std::unique_ptr<GraphBuilder, decltype(deleter)>;

			return ScopedClear{ this, deleter };
		}

		bool areVerticesInserted() const;

	private:
		StringArray vertexIDs;
		GraphPtr result;
		FileParser parser;
	};
}

#endif //__GRAPH_BUILDER_H_INCLUDED__