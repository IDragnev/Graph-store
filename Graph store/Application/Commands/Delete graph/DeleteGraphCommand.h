#ifndef __DELETE_GRAPH_COMMAND_H_INCLUDED__
#define __DELETE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class DeleteGraphCommand : public Command
{
public:
	DeleteGraphCommand() = default;
	virtual ~DeleteGraphCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	DeleteGraphCommand(DeleteGraphCommand&&) = delete;
	DeleteGraphCommand(const DeleteGraphCommand&) = delete;
	DeleteGraphCommand& operator=(DeleteGraphCommand&&) = delete;
	DeleteGraphCommand& operator=(const DeleteGraphCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setIdIfMatched(StringPositional& id);

private:
	String graphID;
};

#endif //__DELETE_GRAPH_COMMAND_H_INCLUDED__
