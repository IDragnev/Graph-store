#ifndef __SAVE_COMMAND_H_INCLUDED__
#define __SAVE_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\Containers\Dynamic Array\DArray.h"

namespace args
{
	template <typename T,
		      template <typename...> class List,
		      typename Reader>
	class PositionalList;
}

namespace IDragnev
{
	namespace GraphStore
	{
		class SaveCommand : public Command
		{
		private:
			using IDList = Containers::DArray<String>;
			using PositionalList = args::PositionalList<String, Containers::DArray, StringReader>;

		public:
			using Command::Command;

			const char* getName() const noexcept override;
			const char* getDescription() const noexcept override;

		private:
			void parseArguments(args::Subparser& parser) override;
			void execute() const override;

			void setIDs(PositionalList& args);

		private:
			IDList graphIDs;
		};
	}
}

#endif //__SAVE_COMMAND_H_INCLUDED__
