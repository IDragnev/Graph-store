#include "RemoveGraphCommand.h"
#include "Application\Command registrator\CommandRegistrator.h"
#include "Serialization\Serialization.h"
#include "Functional\Functional.h"
#include "String\String.h"
#include "UtilityFunctions.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace IDragnev::GraphStore
{
	static CommandRegistrator<RemoveGraphCommand> registrator;

	void RemoveGraphCommand::parseArguments(args::Subparser& parser)
	{
		auto arguments = PositionalList{ parser, "IDs", "The IDs of all the graphs to be removed" };
		parser.Parse();
		setIDs(arguments);
	}

	void RemoveGraphCommand::setIDs(PositionalList& IDs)
	{
		using args::get;

		if (IDs.Matched())
		{
			graphIDs = std::move(get(IDs));
		}
		else
		{
			throw MissingArgument{ IDs.Name() };
		}
	}

	void RemoveGraphCommand::execute()
	{
		using Utility::CallOnDestruction;
 
		auto safeClear = CallOnDestruction{ [this]() noexcept { clear(); } };
		removeGraphsAndTheirFiles();
	}

	void RemoveGraphCommand::removeGraphsAndTheirFiles() const
	{
		auto removeGraphAndItsFile = [](const auto& graphID)
		{
			using Serialization::correspondingGraphFile;

			removeGraph(graphID);
			auto filename = correspondingGraphFile(graphID);
			fs::remove(static_cast<const char*>(filename));
		};
		
		std::for_each(std::begin(graphIDs), std::end(graphIDs), removeGraphAndItsFile);
	}

	void RemoveGraphCommand::clear() noexcept
	{
		graphIDs.clear();
	}

	const char* RemoveGraphCommand::getName() const noexcept
	{
		return "REMOVE-GRAPH";
	}

	const char* RemoveGraphCommand::getDescription() const noexcept
	{
		return "Removes the specified graphs";
	}
}