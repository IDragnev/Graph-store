#ifndef __REMOVE_EDGE_COMMAND_H_INCLUDED__
#define __REMOVE_EDGE_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class RemoveEdgeCommand : public Command
{
public:
	RemoveEdgeCommand() = default;
	virtual ~RemoveEdgeCommand() override = default;

	virtual void execute(args::Subparser& parser) override;
	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	RemoveEdgeCommand(RemoveEdgeCommand&&) = delete;
	RemoveEdgeCommand(const RemoveEdgeCommand&) = delete;
	RemoveEdgeCommand& operator=(RemoveEdgeCommand&&) = delete;
	RemoveEdgeCommand& operator=(const RemoveEdgeCommand&) = delete;

	void parseArguments(args::Subparser& parser);
	void setStartIdIfMatched(StringPositional& startID);
	void setEndIdIfMatched(StringPositional& endID);
	void execute() const;

private:
	String startVertexID;
	String endVertexID;
};

#endif //__REMOVE_EDGE_COMMAND_H_INCLUDED__