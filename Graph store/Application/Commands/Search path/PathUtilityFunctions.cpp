#include "PathUtilityFunctions.h"


void printPath(const Vertex& start, const Vertex& end)
{
	if (start == end)
	{
		std::cout << end.getID();
	}
	else if (!end.getPredecessor())
	{
		printNonExistingPath(start, end);
	}
	else
	{
		printPath(start, *end.getPredecessor());
		std::cout << " , " << end.getID();
	}
}


void printNonExistingPath(const Vertex& start, const Vertex& end)
{
	std::cout << "There is no path between " << start.getID() << " and " << end.getID() << std::endl;
}