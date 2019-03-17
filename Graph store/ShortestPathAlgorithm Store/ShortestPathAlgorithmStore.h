#ifndef __SHORTEST_PATH_ALG_STORE_H_INCLUDED__
#define __SHORTEST_PATH_ALG_STORE_H_INCLUDED__

#include "..\Containers\Dynamic Array\DArray.h"

namespace IDragnev
{
	class String;

	namespace GraphStore
	{
		class ShortestPathAlgorithm;

		class ShortestPathAlgorithmStore
		{
		private:
			using Collection = Containers::DArray<ShortestPathAlgorithm*>;

		public:
			static ShortestPathAlgorithmStore& instance();

			void insertAlgorithm(ShortestPathAlgorithm& algorithm);
			ShortestPathAlgorithm& getAlgorithm(const String& ID);

		private:
			ShortestPathAlgorithmStore();
			~ShortestPathAlgorithmStore() = default;

			ShortestPathAlgorithmStore(const ShortestPathAlgorithmStore&) = delete;
			ShortestPathAlgorithmStore& operator=(const ShortestPathAlgorithmStore&) = delete;

		private:
			Collection::iterator searchAlgorithm(const String& ID);

			static const std::size_t INITIAL_COLLECTION_SIZE = 3;

		private:
			Collection algorithms;
		};
	}
}

#endif //__SHORTEST_PATH_ALG_STORE_H_INCLUDED__