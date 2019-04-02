#include "GraphSaver.h"
#include "..\..\General Exceptions\NoMemoryAvailable.h"
#include "..\SerializationConstants.h"
#include "..\..\Graph\Base Graph\GraphUtilities.h"
#include "..\..\..\Third party\fmt-5.3.0\include\fmt\format.h"

using namespace IDragnev::GraphStore::SerializationConstants;
using namespace fmt::literals;

namespace IDragnev
{
	namespace GraphStore
	{
		class FailedToOpen : public Exception
		{
		public:
			FailedToOpen(const String& file) :
				Exception{ fmt::format("Failed to open {} for writing", file) }
			{
			}
		};

		//All members are set when entering operator() and cleared when leaving it.
		//Thus they act like local variables and need not be copied!
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
			try
			{
				init(g, filename);
				decorateGraph();
				saveGraphToFile();
				clear();
			}
			catch (std::bad_alloc&)
			{
				clear();
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
			pairs.insert({ index, &v.ID() });
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
			forEachVertex(*graph, [this](const Vertex& v) { writeOnASingleLine(v.ID()); });
		}

		void GraphSaver::writeEdges()
		{
			forEachEdge(*graph, [this](const Edge edge) { writeToFile(edge); });
		}

		void GraphSaver::writeToFile(const Edge& edge)
		{
			auto result = fmt::format("{edgeStart}{startIndex}{delim}{endIndex}{delim}{weight}{edgeEnd}\n",
								 	  "edgeStart"_a = EDGE_START,
									  "startIndex"_a = indexOfID(edge.start()),
									  "delim"_a = EDGE_ATTRIBUTE_DELIMITER,
									  "endIndex"_a = indexOfID(edge.end()),
									  "weight"_a = edge.weight(),
									  "edgeEnd"_a = EDGE_END);
			file << result;
		}

		std::size_t GraphSaver::indexOfID(const Vertex& v) const noexcept
		{
			auto result = map.search(v.ID());
			assert(result != nullptr);
			return result->index;
		}

		void GraphSaver::clear()
		{
			graph = nullptr;
			file.close();
			pairs.empty();
			map.empty();
		}
	}
}