#ifndef __INSERT_VERTEX_COMMAND_H_INCLUDED__
#define __INSERT_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class InsertVertexCommand : public Command
{
public:
	InsertVertexCommand() = default;
	virtual ~InsertVertexCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	InsertVertexCommand(InsertVertexCommand&&) = delete;
	InsertVertexCommand(const InsertVertexCommand&) = delete;
	InsertVertexCommand& operator=(InsertVertexCommand&&) = delete;
	InsertVertexCommand& operator=(const InsertVertexCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setIdIfMatched(StringPositional& id);

private:
	String vertexID;
};

#endif //__INSERT_VERTEX_COMMAND_H_INCLUDED__