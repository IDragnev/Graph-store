#include "GraphSaver.h"
#include "Exceptions\Exceptions.h"
#include "Serialization\Serialization.h"
#include "Graph\Base Graph\GraphUtilities.h"
#include "Third party\fmt-5.3.0\include\fmt\format.h"
#include "UtilityFunctions.h"

using namespace fmt::literals;
using IDragnev::Utility::CallOnDestruction;

namespace IDragnev::GraphStore
{
	class FailedToOpen : public Exception
	{
	public:
		FailedToOpen(const String& file) :
			Exception{ fmt::format("Failed to open {} for writing", file) }
		{
		}
	};

	//All members act like local variables and need not be copied
	GraphSaver::GraphSaver(const GraphSaver&) :
		GraphSaver{}
	{
	}

	GraphSaver& GraphSaver::operator=(const GraphSaver&)
	{
		return *this;
	}

	void GraphSaver::operator()(const Graph& g, const String& filename)
	{
		auto safeClear = CallOnDestruction{ [this]() noexcept { clear(); } };
		tryToSave(g, filename);
	}

	void GraphSaver::tryToSave(const Graph& g, const String& filename)
	{
		try
		{
			init(g, filename);
			decorateGraph();
			saveGraphToFile();
		}
		catch (std::bad_alloc&)
		{
			throw NoMemoryAvailable{};
		}
	}

	void GraphSaver::init(const Graph& g, const String& filename)
	{
		open(filename);
		setupCollections(g.getVerticesCount());
		graph = &g;
	}

	void GraphSaver::open(const String& filename)
	{
		file.open(filename);
		if (!file.good())
		{
			throw FailedToOpen{ filename };
		}
	}

	void GraphSaver::setupCollections(std::size_t verticesCount)
	{
		assert(pairs.isEmpty());
		assert(map.isEmpty());
		pairs.ensureSize(verticesCount);
		map = PairPtrMap{ verticesCount };
	}

	void GraphSaver::decorateGraph()
	{
		forEachVertex(*graph, [this](const Vertex& v) { registerVertex(v); });
	}

	void GraphSaver::registerVertex(const Vertex& v)
	{
		auto index = pairs.getCount();
		pairs.insertBack({ index, &v.getID() });
		map.insert(&pairs[index]);
	}

	void GraphSaver::saveGraphToFile()
	{
		writeOnASingleLine(graph->getType());
		writeOnASingleLine(graph->getID());
		writeOnASingleLine(graph->getVerticesCount());
		writeVertexIDs();
		writeOnASingleLine(graph->getEdgesCount());
		writeEdges();
	}

	void GraphSaver::writeVertexIDs()
	{
		forEachVertex(*graph, [this](const Vertex& v) { writeOnASingleLine(v.getID()); });
	}

	void GraphSaver::writeEdges()
	{
		forEachEdge(*graph, [this](const Edge edge) { writeToFile(edge); });
	}

	void GraphSaver::writeToFile(const Edge& edge)
	{
		using Utility::toUnderlyingType;
		using Serialization::EdgeFormat;

		auto result = fmt::format("{edgeStart}{startIndex}{attrDelim}{endIndex}{attrDelim}{weight}{edgeEnd}{edgeDelim}",
			"edgeStart"_a = toUnderlyingType(EdgeFormat::edgeStart),
			"startIndex"_a = indexOfID(edge.start()),
			"attrDelim"_a = toUnderlyingType(EdgeFormat::attributeDelimiter),
			"endIndex"_a = indexOfID(edge.end()),
			"weight"_a = edge.weight(),
			"edgeEnd"_a = toUnderlyingType(EdgeFormat::edgeEnd),
			"edgeDelim"_a = toUnderlyingType(EdgeFormat::edgeDelimiter));
		file << result;
	}

	std::size_t GraphSaver::indexOfID(const Vertex& v) const noexcept
	{
		auto result = map.search(v.getID());
		assert(result != nullptr);
		return result->index;
	}

	void GraphSaver::clear() noexcept
	{
		graph = nullptr;
		file.close();
		file.clear();
		pairs.clear();
		map.clear();
	}
}
