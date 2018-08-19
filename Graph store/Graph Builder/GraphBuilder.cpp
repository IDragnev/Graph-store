#include "GraphBuilder.h"
#include "..\Graph Factory\GraphFactory.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\NoMemoryAvailable.h"


GraphBuilder::GraphPtr GraphBuilder::buildFromFile(const String& filename)
{
	try
	{
		parseFile(filename);
		buildResultFromParsedFile();
		clearParsedState();

		return std::move(result);
	}
	catch (Exception& e)
	{
		handleErrorIn(filename, e);
	}
	catch (std::bad_alloc&)
	{
		handleErrorIn(filename, NoMemoryAvailable{});
	}
}


void GraphBuilder::handleErrorIn(const String& filename, const Exception& exception)
{
	clearState();
	throw Exception{ "Failed to load" + filename + " : " + exception.what() };
}


void GraphBuilder::parseFile(const String& filename)
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
	vertexIDs = parseVertexIds();
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
	RawEdge result{};

	parser.ignoreUntil(EDGE_START);
	result.startVertexIDIndex = parseUnsignedAndIgnoreUntil(EDGE_ATTRIBUTE_DELIMITER);
	result.endVertexIDIndex = parseUnsignedAndIgnoreUntil(EDGE_ATTRIBUTE_DELIMITER);
	result.weight = parseUnsignedAndIgnoreUntil(EDGE_END);
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
	result = GraphFactory::instance().createEmptyGraph(graphType, graphId);
}


void GraphBuilder::insertVertices()
{
	auto constIterator = vertexIDs.getBeginConstIterator();

	while (constIterator)
	{
		result->insertVertexWithID(*constIterator);
		++constIterator;
	}
}


void GraphBuilder::insertEdges()
{
	assert(result->getVerticesCount() == vertexIDs.getCount());

	auto constIterator = edges.getBeginConstIterator();

	while (constIterator)
	{
		insertSingleEdge(*constIterator);
		++constIterator;
	}
}


void GraphBuilder::insertSingleEdge(const RawEdge& edge)
{
	Vertex& start = getVertex(edge.startVertexIDIndex);
	Vertex& end = getVertex(edge.endVertexIDIndex);
	result->insertEdge(start, end, edge.weight);
}


Vertex& GraphBuilder::getVertex(size_t idIndex)
{
	return result->getVertex(vertexIDs[idIndex]);
}


void GraphBuilder::clearParsedState()
{
	graphType = "";
	graphId = "";
	vertexIDs.empty();
	edges.empty();
}


void GraphBuilder::clearState()
{
	clearParsedState();
	result = nullptr;
}