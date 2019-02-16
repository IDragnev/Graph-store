#ifndef __FIXED_SIZE_QUEUE_H_INCLUDED__
#define __FIXED_SIZE_QUEUE_H_INCLUDED__

#include "..\Dynamic Array\DArray.h"

namespace IDragnev
{
	namespace Containers
	{
		template <typename T>
		class FixedSizeQueue
		{
		public:
			template <typename InputIt>
			FixedSizeQueue(InputIt first, InputIt last);
			explicit FixedSizeQueue(std::size_t size);
			FixedSizeQueue(std::initializer_list<T> source);
			FixedSizeQueue(const FixedSizeQueue& source) = default;
			~FixedSizeQueue() = default;

			FixedSizeQueue& operator=(const FixedSizeQueue& rhs) = default;

			void enqueue(T&& item);
			void enqueue(const T& item);
			
			T dequeue();
			const T peekHead() const;

			bool isFull() const noexcept;
			bool isEmpty() const noexcept;
			void empty() noexcept;

		private:
			template <typename Item>
			void insertAsTail(Item&& item);

			std::size_t increment(std::size_t n) const noexcept;

		private:
			std::size_t head = 0;
			std::size_t tail = 0; 
			DArray<T> items;
		};
	}
}

#include "FixedSizeQueueImpl.hpp"
#endif //__FIXED_SIZE_QUEUE_H_INCLUDED__