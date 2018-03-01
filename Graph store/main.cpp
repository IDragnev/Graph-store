#include <iostream>
#include "Vertex/Adjacency list/ForwardList.h"

void fillListAddingTail(ForwardList<int>& list,int count, int first = 0, int step = 1)
{
	for (int i = first; i < count; i += step)
		list.addAsTail(i);
}


int main()
{

	ForwardList<int> list;

	fillListAddingTail(list, 10, 0, 1);

	ForwardList<int> list2(std::move(list));

	ForwardListIterator<int> iterator = list2.getHead();

	for (; iterator; ++iterator)
		std::cout << *iterator << ' ';

	std::cout << std::endl;

	if (!list.isEmpty())
		std::cout << "ERROR!" << std::endl;

	return 0;
}