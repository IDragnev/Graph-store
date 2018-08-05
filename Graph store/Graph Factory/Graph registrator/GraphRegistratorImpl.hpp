#include "../GraphFactory.h"

template <typename GraphType>
GraphRegistrator<GraphType>::GraphRegistrator(const String& createdGraphType) :
	graphCreator(createdGraphType)
{
	GraphFactory::registerCreator(&graphCreator);
}
