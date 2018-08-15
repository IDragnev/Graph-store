#ifndef __SHORTEST_PATH_ALG_REGISTRATOR_H_INCLUDED__
#define __SHORTEST_PATH_ALG_REGISTRATOR_H_INCLUDED__

class String;

template <typename ConcreteAlgorithm>
class ShortestPathAlgorithmRegistrator
{
public:
	ShortestPathAlgorithmRegistrator(const String& ID);
	~ShortestPathAlgorithmRegistrator() = default;

private:
	ShortestPathAlgorithmRegistrator(ShortestPathAlgorithmRegistrator&&) = delete;
	ShortestPathAlgorithmRegistrator(const ShortestPathAlgorithmRegistrator&) = delete;
	ShortestPathAlgorithmRegistrator& operator=(ShortestPathAlgorithmRegistrator&&) = delete;
	ShortestPathAlgorithmRegistrator& operator=(const ShortestPathAlgorithmRegistrator&) = delete;

private:
	ConcreteAlgorithm algorithm;
};

#include "ShortestPathAlgorithmRegistratorImpl.hpp"
#endif //__SHORTEST_PATH_ALG_REGISTRATOR_H_INCLUDED__