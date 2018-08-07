#include "../GraphFactory.h"

template <typename GraphType>
GraphRegistrator<GraphType>::GraphRegistrator(const String& createdGraphType) :
	graphCreator(createdGraphType)
{
	GraphFactory::instance().registerCreator(&graphCreator);
}
