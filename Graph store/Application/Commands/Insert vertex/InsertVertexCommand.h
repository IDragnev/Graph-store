#ifndef __INSERT_VERTEX_COMMAND_H_INCLUDED__
#define __INSERT_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class InsertVertexCommand : public Command
{
public:
	using Command::Command;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setVertexID(StringPositional& argument);

private:
	String vertexID;
};

#endif //__INSERT_VERTEX_COMMAND_H_INCLUDED__