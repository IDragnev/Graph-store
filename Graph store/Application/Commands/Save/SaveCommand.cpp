#include "SaveCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"

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

		//TODO : ADD A CLEAR FUNCTION TO THE TEMPLATE METHOD AND OVERRIDE IT HERE

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