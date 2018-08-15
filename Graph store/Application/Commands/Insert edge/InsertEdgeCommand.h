#ifndef __INSERT_EDGE_COMMAND_H_INCLUDED__
#define __INSERT_EDGE_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class InsertEdgeCommand : public Command
{
private:
	typedef args::Positional<unsigned> UnsignedPositional;

public:
	InsertEdgeCommand() = default;
	virtual ~InsertEdgeCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	InsertEdgeCommand(InsertEdgeCommand&&) = delete;
	InsertEdgeCommand(const InsertEdgeCommand&) = delete;
	InsertEdgeCommand& operator=(InsertEdgeCommand&&) = delete;
	InsertEdgeCommand& operator=(const InsertEdgeCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setStartIdIfMatched(StringPositional& startID);
	void setEndIdIfMatched(StringPositional& endID);
	void setIfMatched(String& str, StringPositional& argument);
	void setWeight(UnsignedPositional& weight);

	static const unsigned DEFAULT_EDGE_WEIGHT = 1;

private:
	String startVertexID;
	String endVertexID;
	unsigned weight;
};

#endif //__INSERT_EDGE_COMMAND_H_INCLUDED__