#ifndef __USE_GRAPH_COMMAND_H_INCLUDED__
#define __USE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class UseGraphCommand : public Command
{
public:
	using Command::Command;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setGraphID(StringPositional& argument);

private:
	String graphID;
};

#endif //__USE_GRAPH_COMMAND_H_INCLUDED__
