#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"
#include "Dynamic Array\DArray.h"

#include "Singly Linked List\SinglyLinkedList.h"
#include "Graph\Directed Graph\DirectedGraph.h"

#include "Application\Application.h"
#include "Application\Commands\Use graph\UseGraphCommand.h"
#include "Application\Commands\Create graph\CreateGraphCommand.h"

int main()
{
	UseGraphCommand command{};
	CreateGraphCommand command2{};

	Application::instance().insertCommand(command);
	Application::instance().insertCommand(command2);
	Application::instance().run();

	return 0;
}