#ifndef __PRIORITY_QUEUE_HANDLE_H_INLCUDED__
#define __PRIORITY_QUEUE_HANDLE_H_INLCUDED__

#include <cstdint>

namespace Containers
{
	class PriorityQueueHandle
	{
		template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
		friend class PriorityQueue;
	public:
		PriorityQueueHandle() = default;

	private:
		PriorityQueueHandle(int32_t value);

		bool isValid() const;
		operator int32_t() const;

	private:
		int32_t value = -1;
	};
}

#endif //__PRIORITY_QUEUE_HANDLE_H_INLCUDED__
