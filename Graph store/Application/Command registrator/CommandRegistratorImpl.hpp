#include "..\Application.h"


template <typename ConcreteCommand>
CommandRegistrator<ConcreteCommand>::CommandRegistrator() :
	command{}
{
	Application::instance().insertCommand(command);
}