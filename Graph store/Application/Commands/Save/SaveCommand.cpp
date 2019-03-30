#include "SaveCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\..\Command registrator\CommandRegistrator.h"
#include "..\..\..\Serialization\Graph Saver\GraphSaver.h"
#include "..\..\..\Functional\Functional.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<SaveCommand> registrator;

		void SaveCommand::parseArguments(args::Subparser& parser)
		{
			auto arguments = PositionalList{ parser, "IDs", "The IDs of all the graphs to be saved" };
			parser.Parse();
			setIDs(arguments);
		}

		void SaveCommand::setIDs(PositionalList& IDs)
		{
			using args::get;
			graphIDs = IDs.Matched() ? std::move(get(IDs)) : IDList{ getUsedGraph().getID() };
		}

		void SaveCommand::execute()
		{
			save();
			clear();
		}

		void SaveCommand::save() const
		{
			using Functional::superpose;
			using Functional::plus;

			auto saveGraph = superpose(GraphSaver{}, getGraph, plus(".txt"));
			std::for_each(std::cbegin(graphIDs), std::cend(graphIDs), saveGraph);
		}

		void SaveCommand::clear()
		{
			graphIDs.clear();
		}

		const char* SaveCommand::getName() const noexcept
		{
			return "SAVE";
		}

		const char* SaveCommand::getDescription() const noexcept
		{
			return "Saves the specified graphs";
		}
	}
}