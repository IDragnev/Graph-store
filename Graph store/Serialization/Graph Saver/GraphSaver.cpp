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
				throw Exception{ fmt::format("Failed to open {0} for writing", filename) };
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
			//TODO: use fmt::print
			file << EDGE_START
				 << indexOfID(edge.start())
				 << EDGE_ATTRIBUTE_DELIMITER
				 << indexOfID(edge.end())
				 << EDGE_ATTRIBUTE_DELIMITER
				 << edge.weight()
				 << EDGE_END
				 << '\n';
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