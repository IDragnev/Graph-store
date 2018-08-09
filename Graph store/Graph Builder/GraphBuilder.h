#ifndef __GRAPH_BUILDER_H_INCLUDED__
#define __GRAPH_BUILDER_H_INCLUDED__

#include "../Exception/Exception.h"
#include "../Graph/Base Graph/Graph.h"
#include "../String/String.h"
#include "../File parser/FileParser.h"
#include <memory>

class GraphBuilder;
typedef Exception<GraphBuilder> GraphBuilderException;

class GraphBuilder
{
private:
	typedef std::unique_ptr<Graph> GraphPtr;

	struct RawEdge
	{
		size_t startVertexIDIndex;
		size_t endVertexIDIndex;
		unsigned weight;
	};

public:
	GraphBuilder() = default;
	~GraphBuilder() = default;

	GraphPtr buildFromFile(const String& filename);

private:
	void parseFile(const String& filename);
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
	void insertSingleEdge(const RawEdge& edge);
	Vertex& getVertex(size_t idIndex);

	void clearParsedState();
	void clearState();

private:
	GraphBuilder(GraphBuilder&&) = delete;
	GraphBuilder(const GraphBuilder&) = delete;
	GraphBuilder& operator=(GraphBuilder&&) = delete;
	GraphBuilder& operator=(const GraphBuilder&) = delete;

private:
	static const char EDGE_START = '(';
	static const char EDGE_END = ')';
	static const char EDGE_ATTRIBUTE_DELIMITER = ',';
	static const char NEW_LINE = '\n';

private:
	String graphType;
	String graphId;
	DArray<String> vertexIDs;
	DArray<RawEdge> edges;

	GraphPtr result;
	FileParser parser;
};

#endif //__GRAPH_BUILDER_H_INCLUDED__