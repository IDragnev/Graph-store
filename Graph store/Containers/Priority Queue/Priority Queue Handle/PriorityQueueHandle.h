#ifndef __PRIORITY_QUEUE_HANDLE_H_INLCUDED__
#define __PRIORITY_QUEUE_HANDLE_H_INLCUDED__

#include <cstddef>
#include <assert.h>

namespace IDragnev::Containers
{
	class PriorityQueueHandle
	{
	private:
		template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
		friend class PriorityQueue;

		using UnderlyingType = std::size_t;

	public:
		PriorityQueueHandle() noexcept : PriorityQueueHandle{ 0u, false }
		{
		}

	private:
		PriorityQueueHandle(UnderlyingType value, bool isValid = true) noexcept : 
			value{ value },
			isValid{ isValid }
		{
		}

		operator UnderlyingType() const noexcept 
		{
			assert(isValid);
			return value;
		}

	private:
		UnderlyingType value;
		bool isValid;
	};
}

#endif //__PRIORITY_QUEUE_HANDLE_H_INLCUDED__
