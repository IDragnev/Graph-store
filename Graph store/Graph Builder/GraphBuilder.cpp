#include "GraphBuilder.h"
#include "..\Graph Factory\GraphFactory.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\NoMemoryAvailable.h"
#include <algorithm>


GraphBuilder::GraphPtr GraphBuilder::buildFromFile(const String& filename)
{
	try
	{
		init(filename);
		build();
		clean();

		return std::move(result);
	}
	catch (Exception& e)
	{
		handleError(filename, e);
	}
	catch (std::bad_alloc&)
	{
		handleError(filename, NoMemoryAvailable{});
	}
}


void GraphBuilder::init(const String& filename)
{
	parser.openFile(filename);
}


void GraphBuilder::clean()
{
	vertexIDs.empty();
	parser.closeFile();
}


void GraphBuilder::handleError(const String& filename, const Exception& exception)
{
	clean();
	result = nullptr;
	throw Exception{ "Failed to load" + filename + " : " + exception.what() };
}


void GraphBuilder::build()
{
	createEmptyGraph();
	insertVertices();
	insertEdges();
}


void GraphBuilder::createEmptyGraph()
{
	auto graphType = parser.parseLine();
	auto graphID = parser.parseLine();

	result = GraphFactory::instance().createEmptyGraph(graphType, graphID);
}


void GraphBuilder::insertVertices()
{
	parseVertexIDs();

	for (auto&& ID : vertexIDs)
	{
		result->insertVertexWithID(ID);
	}
}


void GraphBuilder::parseVertexIDs()
{
	assert(vertexIDs.isEmpty());
	auto IDsCount = parseUnsignedAndIgnoreUntil(NEW_LINE);
	vertexIDs.ensureSize(IDsCount);

	for (auto i = std::size_t{0}; i < IDsCount; ++i)
	{
		vertexIDs.insert(parser.parseLine());
	}
}


unsigned GraphBuilder::parseUnsignedAndIgnoreUntil(char symbol)
{
	auto result = parser.parseUnsigned();
	parser.ignoreUntil(symbol);

	return result;
}


void GraphBuilder::insertEdges()
{
	assert(areVerticesInserted());
	auto edgesCount = parseUnsignedAndIgnoreUntil(NEW_LINE);
	
	for (auto i = std::size_t{0}; i < edgesCount; ++i)
	{
		insertSingleEdge(parseSingleEdge());
	}
}


bool GraphBuilder::areVerticesInserted() const
{
	return result->getVerticesCount() == vertexIDs.getCount();
}


void GraphBuilder::insertSingleEdge(const RawEdge& edge)
{
	auto& start = getVertex(edge.startVertexIDIndex);
	auto& end = getVertex(edge.endVertexIDIndex);
	result->insertEdge(start, end, edge.weight);
}


Graph::Vertex& GraphBuilder::getVertex(std::size_t idIndex)
{
	return result->getVertex(vertexIDs[idIndex]);
}


GraphBuilder::RawEdge GraphBuilder::parseSingleEdge()
{
	auto result = RawEdge{};

	parser.ignoreUntil(EDGE_START);
	result.startVertexIDIndex = parseUnsignedAndIgnoreUntil(EDGE_ATTRIBUTE_DELIMITER);
	result.endVertexIDIndex = parseUnsignedAndIgnoreUntil(EDGE_ATTRIBUTE_DELIMITER);
	result.weight = parseUnsignedAndIgnoreUntil(EDGE_END);
	parser.ignoreUntil(NEW_LINE);

	return result;
}



