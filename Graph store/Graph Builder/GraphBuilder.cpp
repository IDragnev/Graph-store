#include "GraphBuilder.h"
#include "../Graph Factory/GraphFactory.h"


GraphBuilder::GraphPtr GraphBuilder::buildFromFile(const char* filename)
{
	try
	{
		parseFile(filename);
		buildResultFromParsedFile();
		clearParsedState();

		return std::move(result);
	}
	catch (FileParserException& exception)
	{
		assert(!result.get());
		clearParsedState();
		throw GraphBuilderException(exception.what());
	}
}


void GraphBuilder::parseFile(const char* filename)
{
	parser.openFile(filename);
	parseTypeAndId();
	parseComponents();
	parser.closeFile();
}


void GraphBuilder::parseTypeAndId()
{
	graphType = parser.parseLine();
	graphId = parser.parseLine();
}


void GraphBuilder::parseComponents()
{
	vertexIds = parseVertexIds();
	edges = parseEdges();
}


DArray<String> GraphBuilder::parseVertexIds()
{
	assert(edges.isEmpty());

	size_t idsCount = parseUnsignedAndIgnoreUntil(NEW_LINE);
	DArray<String> result(idsCount, idsCount);

	for (size_t i = 0; i < idsCount; ++i)
	{
		result[i] = parser.parseLine();
	}

	return result;
}


DArray<GraphBuilder::RawEdge> GraphBuilder::parseEdges()
{
	size_t edgesCount = parseUnsignedAndIgnoreUntil(NEW_LINE);
	DArray<RawEdge> result(edgesCount);

	for (size_t i = 0; i < edgesCount; ++i)
	{
		result.insert(parseSingleEdge());
	}

	return result;
}


GraphBuilder::RawEdge GraphBuilder::parseSingleEdge()
{
	RawEdge result;

	parser.ignoreUntil(OPEN_PARENTHESIS);
	result.lhsIdIndex = parseUnsignedAndIgnoreUntil(COMMA);
	result.rhsIdIndex = parseUnsignedAndIgnoreUntil(COMMA);
	result.weight = parseUnsignedAndIgnoreUntil(CLOSE_PARENTHESIS);
	parser.ignoreUntil(NEW_LINE);

	return result;
}


unsigned GraphBuilder::parseUnsignedAndIgnoreUntil(char symbol)
{
	unsigned result = parser.parseInteger<unsigned>();
	parser.ignoreUntil(symbol);

	return result;
}


void GraphBuilder::buildResultFromParsedFile()
{
	createEmptyGraph();
	insertVertices();
	insertEdges();
}


void GraphBuilder::createEmptyGraph()
{
	result = GraphFactory::createEmptyGraph(graphType, graphId);
}


void GraphBuilder::insertVertices()
{
	auto constIterator = vertexIds.getHeadConstIterator();

	while (constIterator)
	{
		result->insertVertexWithID(*constIterator);

		++constIterator;
	}
}


void GraphBuilder::insertEdges()
{
	assert(result->getVerticesCount() == vertexIds.getCount());

	auto constIterator = edges.getHeadConstIterator();

	while (constIterator)
	{
		const RawEdge& edge = *constIterator;
		Vertex& lhs = getVertex(edge.lhsIdIndex);
		Vertex& rhs = getVertex(edge.rhsIdIndex);

		result->insertEdgeFromToWithWeight(lhs, rhs, edge.weight);

		++constIterator;
	}
}


Vertex& GraphBuilder::getVertex(size_t idIndex)
{
	return result->getVertexWithID(vertexIds[idIndex]);
}


void GraphBuilder::clearParsedState()
{
	graphType = "";
	graphId = "";
	vertexIds.empty();
	edges.empty();
}