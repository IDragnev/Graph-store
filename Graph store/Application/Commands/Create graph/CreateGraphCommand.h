#ifndef __CREATE_GRAPH_COMMAND_H_INCLUDED__
#define __CREATE_GRAPH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class StringReader;

class CreateGraphCommand : public Command
{
private:
	typedef args::Positional<String, StringReader> StringPositional;

public:
	CreateGraphCommand() = default;
	virtual ~CreateGraphCommand() override = default;

	virtual void execute(args::Subparser& parser) override;
	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	CreateGraphCommand(CreateGraphCommand&&) = delete;
	CreateGraphCommand(const CreateGraphCommand&) = delete;
	CreateGraphCommand& operator=(CreateGraphCommand&&) = delete;
	CreateGraphCommand& operator=(const CreateGraphCommand&) = delete;

	void parseArguments(args::Subparser& parser);
	void setIdIfMatched(StringPositional& id);
	void setType(StringPositional& type);
	void execute() const;

	static const String DEFAULT_GRAPH_TYPE;

private:
	String graphID;
	String graphType;
};

#endif //__CREATE_GRAPH_COMMAND_H_INCLUDED__