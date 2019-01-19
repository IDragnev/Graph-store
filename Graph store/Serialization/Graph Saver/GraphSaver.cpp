#include "GraphSaver.h"
#include "..\..\General Exceptions\NoMemoryAvailable.h"

namespace IDragnev
{
	namespace GraphStore
	{
		void GraphSaver::operator()(const Graph& g, const String& filename)
		{
			init(g, filename);
			saveGraph();
			clear();
		}

		void GraphSaver::init(const Graph& g, const String& filename)
		{
			open(filename);
			setupCollections(g.getVerticesCount());
			graph = &g;
		}

		void GraphSaver::setupCollections(std::size_t verticesCount)
		{
			assert(pairs.isEmpty());
			assert(map.isEmpty());

			try
			{
				pairs.ensureSize(verticesCount);
				map = PairMap{ verticesCount };
			}
			catch (std::bad_alloc&)
			{
				clear();
				throw NoMemoryAvailable{};
			}
		}

		void GraphSaver::open(const String& filename)
		{
			file.open(filename);
			if (!file.good())
			{
				throw Exception{ "Failed to open" + filename + "for writing" };
			}
		}

		void GraphSaver::saveGraphToFile()
		{
			//writeOnASingleLine(graph->type());
			writeOnASingleLine(graph->getVerticesCount());
			writeVertexIDsBuildingMap();
			writeEdges();
		}

		void GraphSaver::writeVertexIDsBuildingMap()
		{
			auto index = std::size_t{ 0 };
			auto iteratorPtr = graph->getConstIteratorToVertices();

			forEach(*iteratorPtr, [&](const Vertex& v)
			{
				writeOnASingleLine(v.ID());
				registerPair(index++, v.ID());
			});
		}

		void GraphSaver::registerPair(std::size_t index, const String& ID)
		{
			pairs.insert({ index, &ID });
			map.insert(&pairs[index]);
		}

		void GraphSaver::writeEdges()
		{
			auto iteratorPtr = graph->getConstIteratorToEdges();

			forEach(*iteratorPtr, [&](const Edge edge)
			{
				write(edge);
			});
		}

		void GraphSaver::write(const Edge& edge)
		{
			/*
			file << EDGE_START
				<< indexOfID(edge.start())
				<< EDGE_ATTRIBUTE_DELIMITER
				<< indexOfID(edge.end())
				<< EDGE_ATTRIBUTE_DELIMITER
				<< edge.weight()
				<< EDGE_END
				<< "\n";
				*/
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