#include "InsertGraphCommand.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\..\Graph Factory\GraphFactory.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\..\Command registrator\CommandRegistrator.h"
#include <memory>


namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<InsertGraphCommand> registrator;

		const String InsertGraphCommand::DEFAULT_GRAPH_TYPE{ "undirected" };

		void InsertGraphCommand::parseArguments(args::Subparser& parser)
		{
			auto ID = StringPositional{ parser, "ID", "The ID of the graph to be created" };
			auto type = StringPositional{ parser, "type", "The type of the graph to be created" };
			parser.Parse();

			setGraphID(ID);
			setGraphType(type);
		}

		void InsertGraphCommand::setGraphID(StringPositional& argument)
		{
			setIfMatched(graphID, argument);
		}

		void InsertGraphCommand::setGraphType(StringPositional& argument)
		{
			if (argument)
			{
				using args::get;
				graphType = std::move(get(argument));
			}
			else
			{
				graphType = DEFAULT_GRAPH_TYPE;
			}
		}

		void InsertGraphCommand::execute() const
		{
			auto& factory = GraphFactory::instance();
			auto graphPtr = factory.createEmptyGraph(graphType, graphID);

			Command::insertGraph(std::move(graphPtr));
			Command::useGraph(graphID);
		}

		const char* InsertGraphCommand::getName() const noexcept
		{
			return "INSERT-GRAPH";
		}

		const char* InsertGraphCommand::getDescription() const noexcept
		{
			return "Inserts a new graph with specified ID and type";
		}
	}
}