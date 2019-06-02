#include "GraphBuilder.h"
#include "Graph Factory\GraphFactory.h"
#include "Serialization\Serialization.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "Third party\fmt-5.3.0\include\fmt\ostream.h"
#include "UtilityFunctions.h"
#include "Exceptions\Exceptions.h"
#include <algorithm>

using namespace fmt::literals;
using IDragnev::Utility::CallOnDestruction;

namespace IDragnev::GraphStore
{
	class FailedToLoad : public Exception
	{
	public:
		FailedToLoad(const String& filename, const Exception& e) :
			Exception{ fmt::format("Failed to load {file} : {reason}",
								   "file"_a = filename,
								   "reason"_a = e.what()) }
		{
		}
	};

	//all members act like local variables
	//and need not be copied
	GraphBuilder::GraphBuilder(const GraphBuilder&) :
		GraphBuilder{}
	{
	}

	GraphBuilder& GraphBuilder::operator=(const GraphBuilder&)
	{
		return *this;
	}

	auto GraphBuilder::operator()(const String& filename) -> GraphPtr
	{
		auto safeClear = CallOnDestruction{ [this]() noexcept { clear(); } };
		tryToBuildFrom(filename);

		return std::move(result);
	}

	void GraphBuilder::clear() noexcept
	{
		vertexIDs.clear();
		parser.closeFile();
		result = nullptr;
	}

	void GraphBuilder::tryToBuildFrom(const String& filename)
	{
		try
		{
			init(filename);
			build();
		}
		catch (Exception& e)
		{
			throw FailedToLoad{ filename, e };
		}
		catch (std::bad_alloc&)
		{
			throw FailedToLoad{ filename, NoMemoryAvailable{} };
		}
	}

	void GraphBuilder::init(const String& filename)
	{
		parser.openFile(filename);
	}

	void GraphBuilder::build()
	{
		createEmptyGraph();
		insertVertices();
		insertEdges();
	}

	void GraphBuilder::createEmptyGraph()
	{
		using GraphIDRef = GraphFactory::GraphIDRef;
		using GraphTypeRef = GraphFactory::GraphTypeRef;

		auto[type, id] = parseTypeAndID();
		auto& factory = GraphFactory::instance();

		result = factory.createEmptyGraph(GraphTypeRef{ type }, GraphIDRef{ id });
	}

	std::pair<String, String> GraphBuilder::parseTypeAndID()
	{
		auto type = parser.parseLine();
		auto id = parser.parseLine();

		return { std::move(type), std::move(id) };
	}

	void GraphBuilder::insertVertices()
	{
		parseVertexIDs();

		for (const auto& ID : vertexIDs)
		{
			result->insertVertexWithID(ID);
		}
	}

	void GraphBuilder::parseVertexIDs()
	{
		assert(vertexIDs.isEmpty());

		auto IDsCount = parseUnsignedAndIgnoreUntil('\n');
		vertexIDs.ensureSize(IDsCount);

		while (IDsCount > 0u)
		{
			vertexIDs.insertBack(parser.parseLine());
			--IDsCount;
		}
	}

	std::size_t GraphBuilder::parseUnsignedAndIgnoreUntil(char symbol)
	{
		auto result = parser.parseUnsigned<std::size_t>();
		parser.ignoreUntil(symbol);

		return result;
	}

	void GraphBuilder::insertEdges()
	{
		assert(areVerticesInserted());

		auto edgesCount = parseUnsignedAndIgnoreUntil('\n');

		while (edgesCount > 0)
		{
			insertSingleEdge(parseSingleEdge());
			--edgesCount;
		}
	}

	bool GraphBuilder::areVerticesInserted() const noexcept
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
		ignoreUntil(EdgeFormat::edgeStart);
		auto startIDIndex = parseUnsignedAndIgnoreUntil(EdgeFormat::attributeDelimiter);
		auto endIDIndex = parseUnsignedAndIgnoreUntil(EdgeFormat::attributeDelimiter);
		auto weight = EdgeWeight{ parseUnsignedAndIgnoreUntil(EdgeFormat::edgeEnd) };
		ignoreUntil(EdgeFormat::edgeDelimiter);

		return { startIDIndex, endIDIndex, weight };
	}

	std::size_t GraphBuilder::parseUnsignedAndIgnoreUntil(EdgeFormat symbol)
	{
		using Utility::toUnderlyingType;
		return parseUnsignedAndIgnoreUntil(toUnderlyingType(symbol));
	}

	void GraphBuilder::ignoreUntil(EdgeFormat symbol)
	{
		using Utility::toUnderlyingType;
		parser.ignoreUntil(toUnderlyingType(symbol));
	}
}

