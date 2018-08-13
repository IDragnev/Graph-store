#ifndef __COMMAND_REGISTRATOR_H_INCLUDED__
#define __COMMAND_REGISTRATOR_H_INCLUDED__

#include "..\Commands\Base\Command.h"

template <typename ConcreteCommand>
class CommandRegistrator
{
public:
	CommandRegistrator();
	~CommandRegistrator() = default;

private:
	CommandRegistrator(CommandRegistrator&&) = delete;
	CommandRegistrator(const CommandRegistrator&) = delete;
	CommandRegistrator& operator=(CommandRegistrator&&) = delete;
	CommandRegistrator& operator=(const CommandRegistrator&) = delete;

private:
	ConcreteCommand command;
};

#include "CommandRegistratorImpl.hpp"
#endif //__COMMAND_REGISTRATOR_H_INCLUDED__
