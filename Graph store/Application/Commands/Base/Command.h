#ifndef __COMMAND_BASE_H_INCLUDED__
#define __COMMAND_BASE_H_INCLUDED__

#include "..\..\..\String\String.h"
#include "StringReader\StringReader.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\..\..\..\..\args\args.hxx"
#include <memory>

class GraphStore;

class Command
{
protected:
	typedef args::Positional<String, StringReader> StringPositional;

public:
	virtual ~Command() = default;

	void execute(args::Subparser& parser);
	virtual const char* getName() const = 0;
	virtual const char* getDescription() const = 0;

	static void setManagedStore(GraphStore& store);

protected:
	Command() = default;

	virtual void parseArguments(args::Subparser& parser) = 0;
	virtual void execute() const = 0;

protected:
	static Graph& getUsedGraph();
	static void useGraph(const String& ID);
	static void removeGraph(const String& ID);
	static void insertGraph(std::unique_ptr<Graph> graphPtr);

private:
	static Graph* usedGraph;
	static GraphStore* graphStore;

private:
	Command(Command&&) = delete;
	Command(const Command&) = delete;
	Command& operator=(Command&&) = delete;
	Command& operator=(const Command&) = delete;
};

#endif //__COMMAND_BASE_H_INCLUDED__