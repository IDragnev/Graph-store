#include "InsertEdgeCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace GS = IDragnev::GraphStore;

static GS::CommandRegistrator<GS::InsertEdgeCommand> registrator;

namespace IDragnev
{
	namespace GraphStore
	{
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

		void InsertEdgeCommand::setIfMatched(String& str, StringPositional& argument)
		{
			if (argument)
			{
				str = args::get(argument);
			}
			else
			{
				throw MissingArgument{ argument.Name() };
			}
		}

		void InsertEdgeCommand::setWeight(UnsignedPositional& weight)
		{
			if (weight)
			{
				this->weight = args::get(weight);
			}
			else
			{
				this->weight = DEFAULT_EDGE_WEIGHT;
			}
		}

		void InsertEdgeCommand::execute() const
		{
			auto& used = Command::getUsedGraph();
			auto& start = used.getVertex(startVertexID);
			auto& end = used.getVertex(endVertexID);

			used.insertEdge(start, end, weight);
		}

		const char* InsertEdgeCommand::getName() const
		{
			return "INSERT-EDGE";
		}

		const char* InsertEdgeCommand::getDescription() const
		{
			return "Inserts an edge with specified weight between a"
					"specified pair of vertices in the currently used graph";
		}
	}
}