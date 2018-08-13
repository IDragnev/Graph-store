#ifndef __USE_GRAPH_COMMAND_H_INCLUDED__
#define __USE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class UseGraphCommand : public Command
{
public:
	UseGraphCommand() = default;
	virtual ~UseGraphCommand() override = default;

	virtual void execute(args::Subparser& parser) override;
	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	UseGraphCommand(UseGraphCommand&&) = delete;
	UseGraphCommand(const UseGraphCommand&) = delete;
	UseGraphCommand& operator=(UseGraphCommand&&) = delete;
	UseGraphCommand& operator=(const UseGraphCommand&) = delete;

	void parseArguments(args::Subparser& parser);
	void setIdIfMatched(StringPositional& id);
	void execute() const;

private:
	String graphID;
};

#endif //__USE_GRAPH_COMMAND_H_INCLUDED__
