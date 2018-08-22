#ifndef __SHORTEST_PATH_ALG_STORE_H_INCLUDED__
#define __SHORTEST_PATH_ALG_STORE_H_INCLUDED__

#include "..\Dynamic Array\DArray.h"

class ShortestPathAlgorithm;
class String;

class ShortestPathAlgorithmStore
{
private:
	using Collection = DArray<ShortestPathAlgorithm*>;

public:
	static ShortestPathAlgorithmStore& instance();

	void insertAlgorithm(ShortestPathAlgorithm& algorithm);
	ShortestPathAlgorithm& getAlgorithm(const String& ID);

private:
	ShortestPathAlgorithmStore();
	~ShortestPathAlgorithmStore() = default;

	ShortestPathAlgorithmStore(ShortestPathAlgorithmStore&&) = delete;
	ShortestPathAlgorithmStore(const ShortestPathAlgorithmStore&) = delete;
	ShortestPathAlgorithmStore& operator=(ShortestPathAlgorithmStore&&) = delete;
	ShortestPathAlgorithmStore& operator=(const ShortestPathAlgorithmStore&) = delete;

private:
	ShortestPathAlgorithm* searchAlgorithm(const String& ID);

	static const std::size_t INITIAL_COLLECTION_SIZE = 3;

private:
	Collection algorithms;
};

#endif //__SHORTEST_PATH_ALG_STORE_H_INCLUDED__