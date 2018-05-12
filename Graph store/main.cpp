#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"
#include "Dynamic Array\DArray.h"

#include "Singly Linked List\SinglyLinkedList.h"

int main()
{
	SinglyLinkedList<int> list;

	for (int i = 0; i < 10; ++i)
		list.insert(i);

	SinglyLinkedList<int>::ConstIterator iterator = list.getHeadIterator();

	while (iterator)
	{
		std::cout << *iterator << ' ';
		++iterator;
	}
	std::cout << std::endl;

	return 0;
}