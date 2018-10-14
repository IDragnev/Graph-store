#ifndef __PRIORITY_QUEUE_HANDLE_H_INLCUDED__
#define __PRIORITY_QUEUE_HANDLE_H_INLCUDED__

namespace Containers
{
	class PriorityQueueHandle
	{
		template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
		friend class PriorityQueue;
	public:
		PriorityQueueHandle() = default;

	private:
		PriorityQueueHandle(long value);

		bool isValid() const;
		operator const long() const;

	private:
		long value = -1;
	};
}

#endif //__PRIORITY_QUEUE_HANDLE_H_INLCUDED__
