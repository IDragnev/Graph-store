#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

#include "args.hxx"
#include "..\GraphStore\GraphStore.h"
#include "StringSplitter\StringSplitter.h"
#include <forward_list>

class String;
class Command;

namespace Containers
{
	template <typename T>
	class DArray;
}

class Application
{
private:
	using Commands =  std::forward_list<args::Command>;
	using Function = std::function<void(args::Subparser&)>;
	using StringSplitter = StringSplitter<Containers::DArray>;

public:
	static Application& instance();

	void run(const String& directory);
	void insertCommand(Command& command);

private:
	Application();
	~Application() = default;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void insertExitCommand();
	void insertHelpCommand();
	void insertCommand(const char* name, const char* description, Function coroutine);

	void load(const String& directory);
	void run();
	void invokeCommand(const std::string& rawInput);
	static std::string receiveInput();

private:
	args::ArgumentParser parser;
	args::Group commandsGroup;
	StringSplitter splitter;
	Commands commands;
	GraphStore graphs;
	bool receivedExitCommand;
};

#endif //__APPLICATION_H_INCLUDED__