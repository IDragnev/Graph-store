#include "Application\Application.h"
#include "Application\Commands\Use graph\UseGraphCommand.h"
#include "Application\Commands\Create graph\CreateGraphCommand.h"
#include "Application\Commands\Delete graph\DeleteGraphCommand.h"
#include "Application\Commands\Insert vertex\InsertVertexCommand.h"

int main()
{
	UseGraphCommand command{};
	CreateGraphCommand command2{};
	DeleteGraphCommand command3{};
	InsertVertexCommand command4{};

	Application::instance().insertCommand(command);
	Application::instance().insertCommand(command2);
	Application::instance().insertCommand(command3);
	Application::instance().insertCommand(command4);

	Application::instance().run();

	return 0;
}