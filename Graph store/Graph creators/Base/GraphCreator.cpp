#include "GraphCreator.h"


GraphCreator::GraphCreator(const char* createdGraphType) :
	createdGraphType(createdGraphType)
{
}


const String& GraphCreator::getCreatedGraphType() const
{
	return createdGraphType;
}