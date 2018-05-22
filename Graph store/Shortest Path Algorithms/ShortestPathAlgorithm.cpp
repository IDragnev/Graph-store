#include "ShortestPathAlgorithm.h"
#include <assert.h>


ShortestPathAlgorithm::ShortestPathAlgorithm(const char* ID) :
	id(ID)
{
	assert(id != String(""));
}


const String& ShortestPathAlgorithm::getID() const
{
	return id;
}