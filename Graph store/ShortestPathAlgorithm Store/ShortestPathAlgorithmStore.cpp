#include "ShortestPathAlgorithmStore.h"
#include "..\Shortest Path Algorithms\Base\ShortestPathAlgorithm.h"
#include "..\String\String.h"
#include "..\General Exceptions\Exception.h"
#include <assert.h>


ShortestPathAlgorithmStore::ShortestPathAlgorithmStore() :
	algorithms{ INITIAL_COLLECTION_SIZE }
{
}


ShortestPathAlgorithmStore& ShortestPathAlgorithmStore::instance()
{
	static ShortestPathAlgorithmStore theOnlyInstance;

	return theOnlyInstance;
}


void ShortestPathAlgorithmStore::insertAlgorithm(ShortestPathAlgorithm& algorithm)
{
	assert(searchAlgorithm(algorithm.getID()) == nullptr);
	algorithms.insert(&algorithm);
}


ShortestPathAlgorithm& ShortestPathAlgorithmStore::getAlgorithm(const String& ID)
{
	auto result = searchAlgorithm(ID);
	
	if (result)
	{
		return *result;
	}
	else
	{
		throw Exception{ "No such algorithm exists" };
	}
}


ShortestPathAlgorithm* ShortestPathAlgorithmStore::searchAlgorithm(const String& ID)
{
	for (auto&& current : algorithms)
	{
		if (current->getID() == ID)
		{
			return current;
		}
	}

	return nullptr;
}
