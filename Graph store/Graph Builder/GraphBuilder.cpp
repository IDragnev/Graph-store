#include "GraphBuilder.h"
#include "../Graph Factory/GraphFactory.h"


GraphBuilder::GraphPtr GraphBuilder::buildFromFile(const String& filename)
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
	catch (std::bad_alloc&)
	{
		clearState();
		throw GraphBuilderException("No memory available while building a graph from " + filename);
	}
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

	size_t idsCount = parseUnsignedAndIgnoreUntil('\n');
	DArray<String> result(idsCount, idsCount);

	for (size_t i = 0; i < idsCount; ++i)
	{
		result[i] = parser.parseLine();
	}

	return result;
}


DArray<GraphBuilder::RawEdge> GraphBuilder::parseEdges()
{
	size_t edgesCount = parseUnsignedAndIgnoreUntil('\n');
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

	parser.ignoreUntil('(');
	result.vertexFromIDIndex = parseUnsignedAndIgnoreUntil(',');
	result.vertexToIDIndex = parseUnsignedAndIgnoreUntil(',');
	result.weight = parseUnsignedAndIgnoreUntil(')');
	parser.ignoreUntil('\n');

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
	auto constIterator = vertexIDs.getHeadConstIterator();

	while (constIterator)
	{
		result->insertVertexWithID(*constIterator);

		++constIterator;
	}
}


void GraphBuilder::insertEdges()
{
	assert(result->getVerticesCount() == vertexIDs.getCount());

	auto constIterator = edges.getHeadConstIterator();

	while (constIterator)
	{
		insertSingleEdge(*constIterator);

		++constIterator;
	}
}


void GraphBuilder::insertSingleEdge(const RawEdge& edge)
{
	Vertex& from = getVertex(edge.vertexFromIDIndex);
	Vertex& to = getVertex(edge.vertexToIDIndex);

	result->insertEdgeFromToWithWeight(from, to, edge.weight);
}


Vertex& GraphBuilder::getVertex(size_t idIndex)
{
	return result->getVertexWithID(vertexIDs[idIndex]);
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