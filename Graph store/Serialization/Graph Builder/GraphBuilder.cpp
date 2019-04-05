#include "GraphBuilder.h"
#include "..\..\Graph Factory\GraphFactory.h"
#include "..\..\General Exceptions\NoMemoryAvailable.h"
#include "..\EdgeFormat.h"
#include "..\..\..\Third party\fmt-5.3.0\include\fmt\format.h"
#include "..\..\UtilityFunctions.h"
#include <algorithm>

using namespace fmt::literals;

namespace IDragnev
{
	namespace GraphStore
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

		auto GraphBuilder::operator()(const String& filename) -> GraphPtr
		{
			try
			{
				init(filename);
				build();
				clear();

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

		void GraphBuilder::clear()
		{
			vertexIDs.empty();
			parser.closeFile();
		}

		void GraphBuilder::handleError(const String& filename, const Exception& e)
		{
			clear();
			result = nullptr;
			throw FailedToLoad{ filename, e };
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

			auto IDsCount = parseUnsignedAndIgnoreUntil('\n');
			vertexIDs.ensureSize(IDsCount);

			while (IDsCount > 0)
			{
				vertexIDs.insert(parser.parseLine());
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
			RawEdge result{};

			ignoreUntil(EdgeFormat::edgeStart);
			result.startVertexIDIndex = parseUnsignedAndIgnoreUntil(EdgeFormat::attributeDelimiter);
			result.endVertexIDIndex = parseUnsignedAndIgnoreUntil(EdgeFormat::attributeDelimiter);
			result.weight = EdgeWeight{ parseUnsignedAndIgnoreUntil(EdgeFormat::edgeEnd) };
			ignoreUntil(EdgeFormat::edgeDelimiter);

			return result;
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
}

