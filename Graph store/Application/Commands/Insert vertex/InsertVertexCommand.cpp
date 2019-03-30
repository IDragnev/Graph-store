#include "InsertVertexCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<InsertVertexCommand> registrator;

		void InsertVertexCommand::parseArguments(args::Subparser& parser)
		{
			auto ID = StringPositional{ parser, "ID", "The ID of the vertex to be inserted" };
			parser.Parse();
			setIfMatched(vertexID, ID);
		}

		void InsertVertexCommand::execute()
		{
			auto& graph = Command::getUsedGraph();
			graph.insertVertexWithID(vertexID);
		}

		const char* InsertVertexCommand::getName() const noexcept
		{
			return "INSERT-VERTEX";
		}

		const char* InsertVertexCommand::getDescription() const noexcept
		{
			return "Inserts a vertex with specified ID to the currently used graph";
		}
	}
}