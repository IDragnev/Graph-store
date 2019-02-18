#ifndef __INSERT_EDGE_COMMAND_H_INCLUDED__
#define __INSERT_EDGE_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class InsertEdgeCommand : public Command
		{
		private:
			using UnsignedPositional = args::Positional<unsigned>;

		public:
			using Command::Command;

			const char* getName() const noexcept override;
			const char* getDescription() const noexcept override;

		private:
			void parseArguments(args::Subparser& parser) override;
			void execute() const override;

			void setStartVertexID(StringPositional& argument);
			void setEndVertexID(StringPositional& argument);
			void setWeight(UnsignedPositional& weight);

			static const std::size_t DEFAULT_EDGE_WEIGHT = 1;

		private:
			String startVertexID;
			String endVertexID;
			std::size_t weight = 0;
		};
	}
}
#endif //__INSERT_EDGE_COMMAND_H_INCLUDED__