#ifndef __INSERT_VERTEX_COMMAND_H_INCLUDED__
#define __INSERT_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class InsertVertexCommand : public Command
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

#endif //__INSERT_VERTEX_COMMAND_H_INCLUDED__