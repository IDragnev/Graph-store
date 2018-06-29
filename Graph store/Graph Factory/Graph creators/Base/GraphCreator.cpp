#include "GraphCreator.h"


GraphCreator::GraphCreator(const char* createdGraphType) :
	createdGraphType(createdGraphType)
{
	assert(this->createdGraphType != String(""));
}


const String& GraphCreator::getCreatedGraphType() const
{
	return createdGraphType;
}