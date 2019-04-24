#include "SaveCommand.h"
#include "Graph\Base Graph\Graph.h"
#include "Application\Command registrator\CommandRegistrator.h"
#include "Serialization\Graph Saver\GraphSaver.h"
#include "Functional\Functional.h"
#include "Serialization\Serialization.h"
#include "UtilityFunctions.h"

namespace IDragnev::GraphStore
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
		using Utility::CallOnDestruction;

		auto safeClear = CallOnDestruction{ [this]() noexcept { clear(); } };
		saveGraphs();
	}

	void SaveCommand::saveGraphs() const
	{
		using Functional::superpose;
		using Serialization::correspondingGraphFile;

		auto saveGraph = superpose(GraphSaver{}, getGraph, correspondingGraphFile);
		std::for_each(std::cbegin(graphIDs), std::cend(graphIDs), saveGraph);
	}

	void SaveCommand::clear() noexcept
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