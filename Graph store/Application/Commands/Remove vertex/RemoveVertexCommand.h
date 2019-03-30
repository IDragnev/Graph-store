#ifndef __REMOVE_VERTEX_COMMAND_H_INCLUDED__
#define __REMOVE_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class RemoveVertexCommand : public Command
		{
		public:
			using Command::Command;

			const char* getName() const noexcept override;
			const char* getDescription() const noexcept override;

		private:
			void parseArguments(args::Subparser& parser) override;
			void execute() override;

		private:
			String vertexID;
		};
	}
}

#endif //__REMOVE_VERTEX_COMMAND_H_INCLUDED__