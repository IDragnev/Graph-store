
namespace IDragnev
{
	namespace Containers
	{
		template <typename T>
		inline FixedSizeQueue<T>::FixedSizeQueue(std::size_t size) :
			items(size + 1, size + 1)
		{
		}

		template <typename T>
		FixedSizeQueue<T>::FixedSizeQueue(std::initializer_list<T> source) :
			FixedSizeQueue(source.size())
		{
			for (auto& item : source)
			{
				items[tail++] = item;
			}
		}

		template <typename T>
		inline void FixedSizeQueue<T>::enqueue(const T& item)
		{
			insertAsTail(item);
		}

		template <typename T>
		inline void FixedSizeQueue<T>::enqueue(T&& item)
		{
			insertAsTail(std::move(item));
		}

		template <typename T>
		template <typename Item>
		void FixedSizeQueue<T>::insertAsTail(Item&& item)
		{
			assert(!isFull()); 
			items[tail] = std::forward<T>(item);
			tail = increment(tail);
		}

		template <typename T>
		inline bool FixedSizeQueue<T>::isFull() const noexcept
		{
			return increment(tail) == head;
		}

		template <typename T>
		inline std::size_t FixedSizeQueue<T>::increment(std::size_t n) const noexcept
		{
			return (n + 1) % items.getSize();
		}

		template <typename T>
		T FixedSizeQueue<T>::dequeue()
		{
			assert(!isEmpty());

			auto result = std::move(items[head]);
			head = increment(head);

			return result;
		}

		template <typename T>
		inline bool FixedSizeQueue<T>::isEmpty() const noexcept
		{
			return head == tail;
		}

		template <typename T>
		inline const T FixedSizeQueue<T>::peekHead() const
		{
			assert(!isEmpty()); 
			return items[head];
		}

		template <typename T>
		inline void FixedSizeQueue<T>::empty() noexcept
		{
			head = tail = 0;
		}
	}
}