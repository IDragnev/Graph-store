#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"
#include "Dynamic Array\DArray.h"
#include <memory>

#include "Queue\Queue.h"


int main()
{
	Queue<int> queue;
	
	for (int i = 10; i >= 0; --i)
		queue.enqueue(i);

	while (!queue.isEmpty())
		std::cout << queue.dequeue() << std::endl;

	return 0;
}