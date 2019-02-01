#include "GraphSaver.h"
#include "..\..\General Exceptions\NoMemoryAvailable.h"
#include "..\SerializationConstants.h"

using namespace IDragnev::GraphStore::SerializationConstants;

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
				throw Exception{ "Failed to open" + filename + "for writing" };
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
			auto index = std::size_t{ 0 };
			auto iteratorPtr = graph->getConstIteratorToVertices();

			forEach(*iteratorPtr, [&](const Vertex& v)
			{
				registerPair(index++, v.ID());
			});
		}

		void GraphSaver::registerPair(std::size_t index, const String& ID)
		{
			pairs.insert({ index, &ID });
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
			auto iteratorPtr = graph->getConstIteratorToVertices();

			forEach(*iteratorPtr, [&](const Vertex& v)
			{
				writeOnASingleLine(v.ID());
			});
		}

		void GraphSaver::writeEdges()
		{
			auto iteratorPtr = graph->getConstIteratorToEdges();

			forEach(*iteratorPtr, [&](const Edge edge)
			{
				file << EDGE_START
					 << indexOfID(edge.start())
					 << EDGE_ATTRIBUTE_DELIMITER
					 << indexOfID(edge.end())
					 << EDGE_ATTRIBUTE_DELIMITER
					 << edge.weight()
					 << EDGE_END
					 << '\n';
			});
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