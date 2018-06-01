#include "ShortestPathAlgorithm.h"
#include "../../Graph/Vertex/Vertex.h"
#include <assert.h>


ShortestPathAlgorithm::ShortestPathAlgorithm(const char* ID) :
	id(ID)
{
	assert(id != String(""));
}


void ShortestPathAlgorithm::findTrivialPathFromTo(Vertex& source, Vertex& goal)
{
	assert(source == goal);

	goal.setPredecessor(nullptr);
	goal.setDistanceToSource(0);
}


const String& ShortestPathAlgorithm::getID() const
{
	return id;
}