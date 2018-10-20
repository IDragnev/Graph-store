#ifndef __PRIORITY_QUEUE_HANDLE_H_INLCUDED__
#define __PRIORITY_QUEUE_HANDLE_H_INLCUDED__

#include <cstdint>

namespace IDragnev
{
	namespace Containers
	{
		class PriorityQueueHandle
		{
			template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
			friend class PriorityQueue;
		public:
			PriorityQueueHandle() = default;

		private:
			PriorityQueueHandle(std::int32_t value);

			bool isValid() const;
			operator std::int32_t() const;

		private:
			std::int32_t value = -1;
		};
	}
}

#endif //__PRIORITY_QUEUE_HANDLE_H_INLCUDED__
