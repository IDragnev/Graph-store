#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"
#include "Dynamic Array\DArray.h"

int main()
{
	DArray<int> darr;

	for (int i = 0; i < 10; ++i)
		darr.insert(i);

	DArray<int>::DArrayIterator<int> iterator = darr.getHeadIterator();

	while (iterator)
	{
		std::cout << *iterator << ' ';
		++iterator;
		iterator++;
	}
	std::cout << std::endl;

	return 0;
}