#ifndef __REMOVE_GRAPH_COMMAND_H_INCLUDED__
#define __REMOVE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class RemoveGraphCommand : public Command
{
public:
	RemoveGraphCommand() = default;
	virtual ~RemoveGraphCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	RemoveGraphCommand(RemoveGraphCommand&&) = delete;
	RemoveGraphCommand(const RemoveGraphCommand&) = delete;
	RemoveGraphCommand& operator=(RemoveGraphCommand&&) = delete;
	RemoveGraphCommand& operator=(const RemoveGraphCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setID(StringPositional& ID);

private:
	String graphID;
};

#endif //__REMOVE_GRAPH_COMMAND_H_INCLUDED__
