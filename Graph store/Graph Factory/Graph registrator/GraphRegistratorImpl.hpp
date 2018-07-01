#include "../GraphFactory.h"

template <typename GraphType>
GraphRegistrator<GraphType>::GraphRegistrator(const char* createdGraphType) :
	graphCreator(createdGraphType)
{
	GraphFactory::registerCreator(&graphCreator);
}
