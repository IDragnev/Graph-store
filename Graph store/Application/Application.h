#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

#include "Commands\Command.h"
#include "..\GraphStore\GraphStore.h"
#include <forward_list>

class Application
{
private:
	typedef std::forward_list<args::Command> Commands;
	typedef std::function<void(args::Subparser&)> Function;

public:
	static Application& instance();

	void run();
	void insertCommand(Command& command);

private:
	Application();
	~Application() = default;

	Application(Application&&) = delete;
	Application(const Application&) = delete;
	Application& operator=(Application&&) = delete;
	Application& operator=(const Application&) = delete;

	void insertExitCommand();
	void insertCommand(const char* name, const char* description, Function coroutine);

private:
	args::ArgumentParser parser;
	args::Group commandsGroup;
	Commands commandsCollection;

	GraphStore graphs;
	bool shouldExit;
};

#endif //__APPLICATION_H_INCLUDED__