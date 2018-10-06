#ifndef __SEARCH_PATH_COMMAND_H_INCLUDED__
#define __SEARCH_PATH_COMMAND_H_INCLUDED__

#include "..\Base\Command.h"
#include "..\..\..\String\String.h"

class SearchPathCommand : public Command 
{
public:
	using Command::Command;

	const char* getName() const override;
	const char* getDescription() const override;

private:
	void parseArguments(args::Subparser& parser) override;
	void execute() const override;

	void setStartVertexID(StringPositional& argument);
	void setEndVertexID(StringPositional& argument);
	void setAlgorithmID(StringPositional& argument);
	void setIfMatched(String& str, StringPositional& argument);

private:
	String startVertexID;
	String endVertexID;
	String algorithmID;
};

#endif //__SEARCH_PATH_COMMAND_H_INCLUDED__