#ifndef __REMOVE_EDGE_COMMAND_H_INCLUDED__
#define __REMOVE_EDGE_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class RemoveEdgeCommand : public Command
{
public:
	RemoveEdgeCommand() = default;
	virtual ~RemoveEdgeCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	RemoveEdgeCommand(RemoveEdgeCommand&&) = delete;
	RemoveEdgeCommand(const RemoveEdgeCommand&) = delete;
	RemoveEdgeCommand& operator=(RemoveEdgeCommand&&) = delete;
	RemoveEdgeCommand& operator=(const RemoveEdgeCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setStartIdIfMatched(StringPositional& startID);
	void setEndIdIfMatched(StringPositional& endID);

private:
	String startVertexID;
	String endVertexID;
};

#endif //__REMOVE_EDGE_COMMAND_H_INCLUDED__