#ifndef __SEARCH_PATH_COMMAND_H_INCLUDED__
#define __SEARCH_PATH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class SearchPathCommand : public Command 
{
public:
	SearchPathCommand() = default;
	virtual ~SearchPathCommand() override = default;

	virtual const char* getName() const override;
	virtual const char* getDescription() const override;

private:
	SearchPathCommand(SearchPathCommand&&) = delete;
	SearchPathCommand(const SearchPathCommand&) = delete;
	SearchPathCommand& operator=(SearchPathCommand&&) = delete;
	SearchPathCommand& operator=(const SearchPathCommand&) = delete;

	virtual void parseArguments(args::Subparser& parser) override;
	virtual void execute() const override;

	void setStartVertexID(StringPositional& startID);
	void setEndVertexID(StringPositional& endID);
	void setAlgorithmID(StringPositional& algorithm);

private:
	String startVertexID;
	String endVertexID;
	String algorithmID;
};

#endif //__SEARCH_PATH_COMMAND_H_INCLUDED__