#ifndef __REMOVE_GRAPH_COMMAND_H_INCLUDED__
#define __REMOVE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class RemoveGraphCommand : public Command
{
public:
	RemoveGraphCommand() = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	RemoveGraphCommand(RemoveGraphCommand&&) = delete;
	RemoveGraphCommand(const RemoveGraphCommand&) = delete;
	RemoveGraphCommand& operator=(RemoveGraphCommand&&) = delete;
	RemoveGraphCommand& operator=(const RemoveGraphCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setGraphID(StringPositional& argument);

private:
	String graphID;
};

#endif //__REMOVE_GRAPH_COMMAND_H_INCLUDED__
