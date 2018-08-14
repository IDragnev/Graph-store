#include "ShortestPathAlgorithmStore.h"


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
	ShortestPathAlgorithm* result = searchAlgorithm(ID);
	
	if (result)
	{
		return *result;
	}
	else
	{
		//TODO: change the exception type
		throw std::runtime_error("No such algorithm exists");
	}
}


ShortestPathAlgorithm* ShortestPathAlgorithmStore::searchAlgorithm(const String& ID)
{
	auto iterator = algorithms.getHeadConstIterator();

	while (!iterator.isFinished())
	{
		ShortestPathAlgorithm* algorithm = iterator.getCurrent();

		if (algorithm->getID() == ID)
		{
			return algorithm;
		}

		iterator.goToNext();
	}

	return nullptr;
}
