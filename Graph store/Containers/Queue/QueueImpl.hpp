
namespace Containers
{
	template <typename T>
	inline bool Queue<T>::isEmpty() const
	{
		return theUnderlyingList.isEmpty();
	}


	template <typename T>
	inline void Queue<T>::empty()
	{
		theUnderlyingList.empty();
	}


	template <typename T>
	inline void Queue<T>::enqueue(const T& item)
	{
		theUnderlyingList.insertAsTail(item);
	}


	template <typename T>
	T Queue<T>::dequeue()
	{
		auto head = peekHead();
		theUnderlyingList.removeHead();

		return head;
	}


	template <typename T>
	inline const T Queue<T>::peekHead() const
	{
		assert(!isEmpty());
		return theUnderlyingList.getHead();
	}
}