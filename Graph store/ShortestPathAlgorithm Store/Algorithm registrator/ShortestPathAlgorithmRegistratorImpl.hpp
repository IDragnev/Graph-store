#include "..\ShortestPathAlgorithmStore.h"
#include "..\..\String\String.h"


template <typename ConcreteAlgorithm>
ShortestPathAlgorithmRegistrator<ConcreteAlgorithm>::ShortestPathAlgorithmRegistrator(const String& ID) :
	algorithm{ ID }
{
	ShortestPathAlgorithmStore::instance().insertAlgorithm(algorithm);
}