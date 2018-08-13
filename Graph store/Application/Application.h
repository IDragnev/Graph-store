#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

#include "Commands\Base\Command.h"
#include "..\GraphStore\GraphStore.h"
#include <forward_list>

class Application
{
private:
	typedef std::forward_list<args::Command> Commands;
	typedef std::function<void(args::Subparser&)> Function;
	typedef std::vector<std::string> InputContainer;

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
	void insertHelpCommand();
	void insertCommand(const char* name, const char* description, Function coroutine);

	static InputContainer receiveInput();

private:
	args::ArgumentParser parser;
	args::Group commandsGroup;
	Commands commands;
	GraphStore graphs;
	bool receivedExitCommand;
};

#endif //__APPLICATION_H_INCLUDED__