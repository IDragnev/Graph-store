#ifndef __INSERT_VERTEX_COMMAND_H_INCLUDED__
#define __INSERT_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class InsertVertexCommand : public Command
{
public:
	InsertVertexCommand() = default;
	virtual ~InsertVertexCommand() override = default;

	virtual void execute(args::Subparser& parser) override;
	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	InsertVertexCommand(InsertVertexCommand&&) = delete;
	InsertVertexCommand(const InsertVertexCommand&) = delete;
	InsertVertexCommand& operator=(InsertVertexCommand&&) = delete;
	InsertVertexCommand& operator=(const InsertVertexCommand&) = delete;

	void parseArguments(args::Subparser& parser);
	void setIdIfMatched(StringPositional& id);
	void execute() const;

private:
	String vertexID;
};

#endif //__INSERT_VERTEX_COMMAND_H_INCLUDED__