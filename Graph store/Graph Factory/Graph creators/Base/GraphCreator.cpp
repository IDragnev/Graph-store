#include "GraphCreator.h"
#include <assert.h>


GraphCreator::GraphCreator(const String& graphType) :
	createdGraphType{ graphType }
{
	assert(graphType != String{ "" });
}


const String& GraphCreator::getCreatedGraphType() const
{
	return createdGraphType;
}