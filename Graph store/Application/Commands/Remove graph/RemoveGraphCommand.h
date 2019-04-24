#ifndef __REMOVE_GRAPH_COMMAND_H_INCLUDED__
#define __REMOVE_GRAPH_COMMAND_H_INCLUDED__

#include "Application\Commands\Base\Command.h"
#include <vector>

namespace args
{
	template <typename T,
		      template <typename...> class List,
		      typename Reader>
	class PositionalList;
}

namespace IDragnev::GraphStore
{
	class RemoveGraphCommand : public Command
	{
	private:
		using IDList = std::vector<String>;
		using PositionalList = args::PositionalList<String, std::vector, StringReader>;

	public:
		using Command::Command;

		const char* getName() const noexcept override;
		const char* getDescription() const noexcept override;

	private:
		void parseArguments(args::Subparser& parser) override;
		void execute() override;

		void removeGraphsAndTheirFiles() const;
		void clear() noexcept;

		void setIDs(PositionalList& args);

	private:
		IDList graphIDs;
	};
}

#endif //__REMOVE_GRAPH_COMMAND_H_INCLUDED__
