#ifndef __PRIORITY_QUEUE_HANDLE_H_INLCUDED__
#define __PRIORITY_QUEUE_HANDLE_H_INLCUDED__

class PriorityQueueHandle
{
	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	friend class PriorityQueue;
public:
	PriorityQueueHandle() = default;

private:
	PriorityQueueHandle(long value);

	static const PriorityQueueHandle& invalidHandle();

	bool isValid() const;
	operator long() const;

private:
	long value = -1;
};

#endif //__PRIORITY_QUEUE_HANDLE_H_INLCUDED__
