#ifndef __INSERT_GRAPH_COMMAND_H_INCLUDED__
#define __INSERT_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class InsertGraphCommand : public Command
		{
		public:
			using Command::Command;

			const char* getName() const override;
			const char* getDescription() const override;

		private:
			void parseArguments(args::Subparser& parser) override;
			void execute() const override;

			void setGraphID(StringPositional& argument);
			void setGraphType(StringPositional& argument);

			static const String DEFAULT_GRAPH_TYPE;

		private:
			String graphID;
			String graphType;
		};
	}
}

#endif //__INSERT_GRAPH_COMMAND_H_INCLUDED__