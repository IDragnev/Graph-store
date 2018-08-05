#include "GraphCreator.h"


GraphCreator::GraphCreator(const String& createdGraphType) :
	createdGraphType(createdGraphType)
{
	assert(createdGraphType != String(""));
}


const String& GraphCreator::getCreatedGraphType() const
{
	return createdGraphType;
}