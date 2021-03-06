#include "ShortestPathAlgorithmStore.h"
#include "..\Shortest Path Algorithms\Base\ShortestPathAlgorithm.h"
#include "..\String\String.h"
#include "Exceptions\Exceptions.h"
#include "ID matcher\IDMatcher.h"
#include <assert.h>
#include <algorithm>

namespace IDragnev
{
	namespace GraphStore
	{
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
			assert(!searchAlgorithm(algorithm.getID()));
			algorithms.insertBack(&algorithm);
		}

		ShortestPathAlgorithm& ShortestPathAlgorithmStore::getAlgorithm(const String& ID)
		{
			if (auto iterator = searchAlgorithm(ID);
				iterator)
			{
				auto alg = *iterator;
				return *alg;
			}
			else
			{
				throw Exception{ "No such algorithm exists" };
			}
		}

		auto ShortestPathAlgorithmStore::searchAlgorithm(const String& ID) -> Collection::iterator
		{
			using AlgorithmID = ConstStringIDRef;
			return std::find_if(std::begin(algorithms), std::end(algorithms), matches(AlgorithmID(ID)));
		}
	}
}