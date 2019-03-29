#include "InsertEdgeCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<InsertEdgeCommand> registrator;

		void InsertEdgeCommand::parseArguments(args::Subparser& parser)
		{
			auto startID = StringPositional{ parser, "startVertexID", "The ID of the start vertex" };
			auto endID = StringPositional{ parser, "endVertexID", "The ID of the end vertex" };
			auto weight = UnsignedPositional{ parser, "weight", "The weight of the edge" };
			parser.Parse();

			setStartVertexID(startID);
			setEndVertexID(endID);
			setWeight(weight);
		}

		void InsertEdgeCommand::setStartVertexID(StringPositional& argument)
		{
			setIfMatched(startVertexID, argument);
		}

		void InsertEdgeCommand::setEndVertexID(StringPositional& argument)
		{
			setIfMatched(endVertexID, argument);
		}

		void InsertEdgeCommand::setWeight(UnsignedPositional& weight)
		{
			this->weight = weight.Matched() ? args::get(weight) : DEFAULT_EDGE_WEIGHT;
		}

		void InsertEdgeCommand::execute() const
		{
			auto& graph = Command::getUsedGraph();
			auto& start = graph.getVertex(startVertexID);
			auto& end = graph.getVertex(endVertexID);

			graph.insertEdge(start, end, weight);
		}

		const char* InsertEdgeCommand::getName() const noexcept
		{
			return "INSERT-EDGE";
		}

		const char* InsertEdgeCommand::getDescription() const noexcept
		{
			return "Inserts an edge with specified weight between a"
					"specified pair of vertices in the currently used graph";
		}
	}
}