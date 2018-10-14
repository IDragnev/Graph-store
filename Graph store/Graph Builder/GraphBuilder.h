#ifndef __GRAPH_BUILDER_H_INCLUDED__
#define __GRAPH_BUILDER_H_INCLUDED__

#include "..\String\String.h"
#include "..\File parser\FileParser.h"
#include "..\Containers\Dynamic Array\DArray.h"
#include "..\Graph\Base Graph\Graph.h"
#include <memory>

class Exception;

class GraphBuilder
{
private:
	using GraphPtr = std::unique_ptr<Graph>;

	struct RawEdge
	{
		std::size_t startVertexIDIndex{};
		std::size_t endVertexIDIndex{};
		unsigned weight{};
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
	unsigned parseUnsignedAndIgnoreUntil(char symbol);
	Graph::Vertex& getVertex(std::size_t idIndex);

	void clean();

	void handleError(const String& filename, const Exception& e);
	bool areVerticesInserted() const;

private:
	static const char EDGE_START = '(';
	static const char EDGE_END = ')';
	static const char EDGE_ATTRIBUTE_DELIMITER = ',';
	static const char NEW_LINE = '\n';

private:
	DArray<String> vertexIDs;
	GraphPtr result;
	FileParser parser;
};

#endif //__GRAPH_BUILDER_H_INCLUDED__