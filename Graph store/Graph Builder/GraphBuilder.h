#ifndef __GRAPH_BUILDER_H_INCLUDED__
#define __GRAPH_BUILDER_H_INCLUDED__

#include "GraphBuilderException/GraphBuilderException.h"
#include "../Graph/Base Graph/Graph.h"
#include "../String/String.h"
#include "../File parser/FileParser.h"
#include <memory>

class GraphBuilder
{
private:
	typedef std::unique_ptr<Graph> GraphPtr;

	struct RawEdge
	{
		size_t lhsIdIndex;
		size_t rhsIdIndex;
		unsigned weight;
	};

public:
	GraphBuilder() = default;
	~GraphBuilder() = default;

	GraphPtr buildFromFile(const char* filename);

private:
	void parseFile(const char* filename);
	void parseTypeAndId();
	void parseComponents();
	DArray<String> parseVertexIds();
	DArray<RawEdge> parseEdges();
	RawEdge parseSingleEdge();
	unsigned parseUnsignedAndIgnoreUntil(char symbol);

	void buildResultFromParsedFile();
	void createEmptyGraph();
	void insertVertices();
	void insertEdges();
	Vertex& getVertex(size_t idIndex);

	void clearParsedState();

private:
	GraphBuilder(GraphBuilder&&) = delete;
	GraphBuilder(const GraphBuilder&) = delete;
	GraphBuilder& operator=(GraphBuilder&&) = delete;
	GraphBuilder& operator=(const GraphBuilder&) = delete;

private:
	static const char OPEN_PARENTHESIS = '(';
	static const char CLOSE_PARENTHESIS = ')';
	static const char NEW_LINE = '\n';
	static const char COMMA = ',';

private:
	String graphType;
	String graphId;
	DArray<String> vertexIds;
	DArray<RawEdge> edges;

	GraphPtr result;
	FileParser parser;
};

#endif //__GRAPH_BUILDER_H_INCLUDED__