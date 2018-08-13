#include "Application\Application.h"
#include "Application\Commands\Use graph\UseGraphCommand.h"
#include "Application\Commands\Create graph\CreateGraphCommand.h"
#include "Application\Commands\Delete graph\DeleteGraphCommand.h"

int main()
{
	UseGraphCommand command{};
	CreateGraphCommand command2{};
	DeleteGraphCommand command3{};

	Application::instance().insertCommand(command);
	Application::instance().insertCommand(command2);
	Application::instance().insertCommand(command3);
	Application::instance().run();

	return 0;
}