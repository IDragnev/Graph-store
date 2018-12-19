#include "ShortestPathAlgorithmStore.h"
#include "..\Shortest Path Algorithms\Base\ShortestPathAlgorithm.h"
#include "..\String\String.h"
#include "..\General Exceptions\Exception.h"
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
			algorithms.insert(&algorithm);
		}

		ShortestPathAlgorithm& ShortestPathAlgorithmStore::getAlgorithm(const String& ID)
		{
			auto iterator = searchAlgorithm(ID);

			if (iterator)
			{
				auto alg = *iterator;
				return *alg;
			}
			else
			{
				throw Exception{ "No such algorithm exists" };
			}
		}

		auto ShortestPathAlgorithmStore::searchAlgorithm(const String& ID) noexcept -> Collection::iterator
		{
			using std::begin;
			using std::end;

			return std::find_if(begin(algorithms),
								end(algorithms), 
								[&](const ShortestPathAlgorithm* alg) { return alg->getID() == ID; });
		}
	}
}