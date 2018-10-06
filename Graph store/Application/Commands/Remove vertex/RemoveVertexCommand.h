#ifndef __REMOVE_VERTEX_COMMAND_H_INCLUDED__
#define __REMOVE_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class RemoveVertexCommand : public Command
{
public:
	using Command::Command;

	const char* getName() const override;
	const char* getDescription() const override;

private:
	void parseArguments(args::Subparser& parser) override;
	void execute() const override;

	void setVertexID(StringPositional& argument);

private:
	String vertexID;
};

#endif //__REMOVE_VERTEX_COMMAND_H_INCLUDED__