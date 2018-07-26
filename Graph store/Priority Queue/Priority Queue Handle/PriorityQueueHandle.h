#ifndef __PRIORITY_QUEUE_HANDLE_H_INLCUDED__
#define __PRIORITY_QUEUE_HANDLE_H_INLCUDED__

class PriorityQueueHandle
{
	template <typename Item, typename Key, typename CompareFunction, typename HandleSetter>
	friend class PriorityQueue;
public:
	PriorityQueueHandle();

private:
	PriorityQueueHandle(long value);

	static const PriorityQueueHandle& invalidHandle();

	bool isValid() const;
	operator long() const;

private:
	static const long INVALID_HANDLE_VALUE = -1;

private:
	long value;
};

#endif //__PRIORITY_QUEUE_HANDLE_H_INLCUDED__
