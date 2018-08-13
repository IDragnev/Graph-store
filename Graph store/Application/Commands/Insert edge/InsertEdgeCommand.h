#ifndef __INSERT_EDGE_COMMAND_H_INCLUDED__
#define __INSERT_EDGE_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"

class InsertEdgeCommand : public Command
{
private:
	typedef args::Positional<unsigned> UnsignedPositional;

public:
	InsertEdgeCommand() = default;
	virtual ~InsertEdgeCommand() override = default;

	virtual void execute(args::Subparser& parser) override;
	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	InsertEdgeCommand(InsertEdgeCommand&&) = delete;
	InsertEdgeCommand(const InsertEdgeCommand&) = delete;
	InsertEdgeCommand& operator=(InsertEdgeCommand&&) = delete;
	InsertEdgeCommand& operator=(const InsertEdgeCommand&) = delete;

	void parseArguments(args::Subparser& parser);
	void setStartIdIfMatched(StringPositional& startID);
	void setEndIdIfMatched(StringPositional& endID);
	void setWeight(UnsignedPositional& weight);
	void execute() const;

	static const unsigned DEFAULT_EDGE_WEIGHT = 1;

private:
	String startVertexID;
	String endVertexID;
	unsigned weight;
};

#endif //__INSERT_EDGE_COMMAND_H_INCLUDED__