#ifndef __SEARCH_PATH_COMMAND_H_INCLUDED__
#define __SEARCH_PATH_COMMAND_H_INCLUDED__

#include "Application\Commands\Base\Command.h"
#include "String\String.h"

namespace IDragnev::GraphStore
{
	class SearchPathCommand : public Command
	{
	public:
		using Command::Command;

		const char* getName() const noexcept override;
		const char* getDescription() const noexcept override;

	private:
		void parseArguments(args::Subparser& parser) override;
		void execute() override;

	private:
		String startVertexID;
		String endVertexID;
		String algorithmID;
	};
}

#endif //__SEARCH_PATH_COMMAND_H_INCLUDED__