#ifndef __INSERT_GRAPH_COMMAND_H_INCLUDED__
#define __INSERT_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class InsertGraphCommand : public Command
{
public:
	InsertGraphCommand() = default;
	virtual ~InsertGraphCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	InsertGraphCommand(InsertGraphCommand&&) = delete;
	InsertGraphCommand(const InsertGraphCommand&) = delete;
	InsertGraphCommand& operator=(InsertGraphCommand&&) = delete;
	InsertGraphCommand& operator=(const InsertGraphCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setGraphID(StringPositional& argument);
	void setGraphType(StringPositional& argument);

	static const String DEFAULT_GRAPH_TYPE;

private:
	String graphID;
	String graphType;
};

#endif //__INSERT_GRAPH_COMMAND_H_INCLUDED__