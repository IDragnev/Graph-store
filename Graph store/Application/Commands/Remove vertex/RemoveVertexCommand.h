#ifndef __REMOVE_VERTEX_COMMAND_H_INCLUDED__
#define __REMOVE_VERTEX_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class RemoveVertexCommand : public Command
{
public:
	RemoveVertexCommand() = default;
	virtual ~RemoveVertexCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	RemoveVertexCommand(RemoveVertexCommand&&) = delete;
	RemoveVertexCommand(const RemoveVertexCommand&) = delete;
	RemoveVertexCommand& operator=(RemoveVertexCommand&&) = delete;
	RemoveVertexCommand& operator=(const RemoveVertexCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setVertexID(StringPositional& argument);

private:
	String vertexID;
};

#endif //__REMOVE_VERTEX_COMMAND_H_INCLUDED__