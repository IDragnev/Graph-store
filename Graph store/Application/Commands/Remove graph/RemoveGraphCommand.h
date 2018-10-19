#ifndef __REMOVE_GRAPH_COMMAND_H_INCLUDED__
#define __REMOVE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class RemoveGraphCommand : public Command
		{
		public:
			using Command::Command;

			const char* getName() const override;
			const char* getDescription() const override;

		private:
			void parseArguments(args::Subparser& parser) override;
			void execute() const override;

			void setGraphID(StringPositional& argument);

		private:
			String graphID;
		};
	}
}

#endif //__REMOVE_GRAPH_COMMAND_H_INCLUDED__
