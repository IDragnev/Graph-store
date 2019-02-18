#ifndef __SEARCH_PATH_COMMAND_H_INCLUDED__
#define __SEARCH_PATH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class SearchPathCommand : public Command
		{
		public:
			using Command::Command;

			const char* getName() const noexcept override;
			const char* getDescription() const noexcept override;

		private:
			void parseArguments(args::Subparser& parser) override;
			void execute() const override;

		private:
			String startVertexID;
			String endVertexID;
			String algorithmID;
		};
	}
}

#endif //__SEARCH_PATH_COMMAND_H_INCLUDED__