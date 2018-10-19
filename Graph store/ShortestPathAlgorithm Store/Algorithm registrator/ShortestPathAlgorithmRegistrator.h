#ifndef __SHORTEST_PATH_ALG_REGISTRATOR_H_INCLUDED__
#define __SHORTEST_PATH_ALG_REGISTRATOR_H_INCLUDED__

#include "..\ShortestPathAlgorithmStore.h"
#include "..\..\String\String.h"

namespace IDragnev
{
	namespace GraphStore
	{
		template <typename ConcreteAlgorithm>
		class ShortestPathAlgorithmRegistrator
		{
		public:
			explicit ShortestPathAlgorithmRegistrator(const String& ID)	:
				algorithm { ID }
			{
				ShortestPathAlgorithmStore::instance().insertAlgorithm(algorithm);
			}

			ShortestPathAlgorithmRegistrator(const ShortestPathAlgorithmRegistrator&) = delete;
			ShortestPathAlgorithmRegistrator& operator=(const ShortestPathAlgorithmRegistrator&) = delete;

		private:
			ConcreteAlgorithm algorithm;
		};
	}
}

#endif //__SHORTEST_PATH_ALG_REGISTRATOR_H_INCLUDED__