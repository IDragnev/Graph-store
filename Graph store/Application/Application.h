#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

#include "args.hxx"
#include "..\GraphStore\GraphStore.h"
#include <forward_list>

class String;
class Command;
template <typename T>
class DArray;

class Application
{
private:
	using Commands =  std::forward_list<args::Command>;
	using Function = std::function<void(args::Subparser&)>;
	using InputContainer = DArray<std::string>;

public:
	static Application& instance();

	void run(const String& directory);
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

	void load(const String& directory);
	void run();
	void invokeCommand(const InputContainer& input);
	static InputContainer receiveInput();

private:
	args::ArgumentParser parser;
	args::Group commandsGroup;
	Commands commands;
	GraphStore graphs;
	bool receivedExitCommand;
};

#endif //__APPLICATION_H_INCLUDED__