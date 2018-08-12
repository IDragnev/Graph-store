#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"
#include "Dynamic Array\DArray.h"

#include "Singly Linked List\SinglyLinkedList.h"
#include "Graph\Directed Graph\DirectedGraph.h"

#include "Application\Application.h"


int main()
{
	Application::instance().run();

	return 0;
}