#include "RemoveVertexCommand.h"
#include "Graph\Base Graph\Graph.h"
#include "Application\Command registrator\CommandRegistrator.h"

namespace IDragnev::GraphStore
{
	static CommandRegistrator <RemoveVertexCommand> registrator;

	void RemoveVertexCommand::parseArguments(args::Subparser& parser)
	{
		auto ID = StringPositional{ parser,"ID", "The ID of the vertex to be removed" };
		parser.Parse();
		setIfMatched(vertexID, ID);
	}

	void RemoveVertexCommand::execute()
	{
		auto& graph = Command::getUsedGraph();
		graph.removeVertex(vertexID);
	}

	const char* RemoveVertexCommand::getName() const noexcept
	{
		return "REMOVE-VERTEX";
	}

	const char* RemoveVertexCommand::getDescription() const noexcept
	{
		return "Removes a specified vertex from the currently used graph";
	}
}