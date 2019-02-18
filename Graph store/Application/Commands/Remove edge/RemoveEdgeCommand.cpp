#include "RemoveEdgeCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<RemoveEdgeCommand> registrator;

		void RemoveEdgeCommand::parseArguments(args::Subparser& parser)
		{
			auto startID = StringPositional{ parser, "startVertexID", "The ID of the start vertex" };
			auto endID = StringPositional{ parser, "endVertexID", "The ID of the end vertex" };
			parser.Parse();

			setIfMatched(startVertexID, startID);
			setIfMatched(endVertexID, endID);
		}

		void RemoveEdgeCommand::execute() const
		{
			auto& graph = Command::getUsedGraph();
			auto& start = graph.getVertex(startVertexID);
			auto& end = graph.getVertex(endVertexID);

			graph.removeEdge(start, end);
		}

		const char* RemoveEdgeCommand::getName() const noexcept
		{
			return "REMOVE-EDGE";
		}

		const char* RemoveEdgeCommand::getDescription() const noexcept
		{
			return "Removes the edge between a specified pair of vertices in the currently used graph";
		}
	}
}