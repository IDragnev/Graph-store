#include "..\..\UtilityFunctions.h"

namespace IDragnev
{
	namespace Containers
	{
		template <typename T>
		DArray<T>::DArray() noexcept :
			size{ 0 },
			count{ 0 },
			items{ nullptr }
		{
		}

		template <typename T>
		DArray<T>::DArray(std::initializer_list<T> source) :
			DArray(source.begin(), source.end())
		{
		}

		template <typename T>
		DArray<T>::DArray(const DArray<T>& source) :
			DArray(source.cbegin(), source.cend())
		{
		}

		template <typename T>
		template <typename InputIt, typename>
		DArray<T>::DArray(InputIt first, InputIt last) :
			DArray(std::distance(first, last))
		{
			for (auto current = first; 
				 current != last; 
				 ++current)
			{
				items[count++] = *current;
			}
		}

		template <typename T>
		DArray<T>::DArray(std::size_t size, std::size_t count) :
			size{ size },
			count{ [count, size] { assert(count <= size); return count; }() },
			items{ [size] { return size > 0 ? new T[size]{} : nullptr; }() }
		{
		}

		template <typename T>
		DArray<T>::DArray(DArray&& source) noexcept :
			count{ source.count },
			size{ source.size },
			items{ source.items }
		{
			source.nullifyMembers();
		}

		template <typename T>
		inline void DArray<T>::nullifyMembers() noexcept
		{
			size = count = 0;
			items = nullptr;
		}

		template <typename T>
		DArray<T>& DArray<T>::operator=(const DArray& rhs)
		{
			if (this != &rhs)
			{
				swapContentsWith(rhs);
			}

			return *this;
		}

		template <typename T>
		DArray<T>& DArray<T>::operator=(DArray&& rhs) noexcept
		{
			if (this != &rhs)
			{
				swapContentsWith(std::move(rhs));
			}

			return *this;
		}

		template <typename T>
		inline void DArray<T>::swapContentsWith(DArray temporary) noexcept
		{
			std::swap(size, temporary.size);
			std::swap(count, temporary.count);
			std::swap(items, temporary.items);
		}

		template <typename T>
		inline DArray<T>::~DArray()
		{
			destroyItems();
		}

		template <typename T>
		inline void DArray<T>::destroyItems() noexcept
		{
			delete[] items;
		}

		template <typename T>
		void DArray<T>::shrink(std::size_t newSize)
		{
			assert(newSize <= size);

			if (newSize == 0)
			{
				clear();
			}
			else if (newSize != size)
			{
				resize(newSize);
			}
		}

		template <typename T>
		void DArray<T>::clear() noexcept
		{
			destroyItems();
			nullifyMembers();
		}

		template <typename T>
		void DArray<T>::resize(std::size_t newSize)
		{
			auto newCount = (count <= newSize) ? count : newSize;
			auto temporary = DArray(newSize, newCount);

			using Utility::moveIfNothrowMoveAssignable;
			for (std::size_t i = 0; i < newCount; ++i)
			{
				temporary.items[i] = moveIfNothrowMoveAssignable(items[i]);
			}

			swapContentsWith(std::move(temporary));
		}

		template <typename T>
		inline void DArray<T>::ensureSize(std::size_t newSize)
		{
			if (newSize > size)
			{
				resize(newSize);
			}
		}

		template <typename T>
		inline void DArray<T>::insertBack(const T& item)
		{
			doInsertBack(item);
		}

		template <typename T>
		inline void DArray<T>::insertBack(T&& item)
		{
			doInsertBack(std::move(item));
		}

		template <typename T>
		template <typename Item>
		void DArray<T>::doInsertBack(Item&& item)
		{
			enlargeIfFull();
			items[count] = std::forward<Item>(item);
			++count;
		}

		template <typename T>
		void DArray<T>::enlargeIfFull()
		{
			assert(count <= size);

			if (count == size)
			{
				resize(size > 0 ? (GROWTH_FACTOR * size) : DEFAULT_SIZE);
			}
		}

		template <typename T>
		inline void DArray<T>::insertAt(std::size_t position, const T& item)
		{
			doInsertAt(position, item);
		}

		template <typename T>
		inline void DArray<T>::insertAt(std::size_t position, T&& item)
		{
			doInsertAt(position, std::move(item));
		}

		template <typename T>
		template <typename Item>
		void DArray<T>::doInsertAt(std::size_t position, Item&& item)
		{
			if (position != count)
			{
				assert(hasItemAt(position));
				enlargeIfFull();

				shiftOnePositionRight(position, count - 1);
				items[position] = std::forward<Item>(item);
				++count;
			}
			else
			{
				insertBack(std::forward<Item>(item));
			}
		}

		template <typename T>
		inline void DArray<T>::shiftOnePositionRight(std::size_t start, std::size_t end)
		{
			assert(end + 1 < size);
			std::move_backward(items + start, items + (end + 1), items + (end + 2));
		}

		template <typename T>
		inline void DArray<T>::removeAt(std::size_t position)
		{
			assert(hasItemAt(position));
			shiftOnePositionLeft(position + 1, --count);
		}

		template <typename T>
		inline void DArray<T>::shiftOnePositionLeft(std::size_t start, std::size_t end)
		{
			assert(start > 0 && end < size);
			std::move(items + start, items + (end + 1), items + (start - 1));
		}

		template <typename T>
		inline T& DArray<T>::operator[](std::size_t position)
		{
			return const_cast<T&>(static_cast<const DArray&>(*this)[position]);
		}

		template <typename T>
		inline const T& DArray<T>::operator[](std::size_t position) const
		{
			assert(hasItemAt(position));
			return items[position];
		}

		template <typename T>
		inline bool DArray<T>::hasItemAt(std::size_t position) const noexcept
		{
			return position < count;
		}

		template <typename T>
		inline bool DArray<T>::isEmpty() const noexcept
		{
			return count == 0;
		}

		template <typename T>
		inline typename auto DArray<T>::getCount() const noexcept -> std::size_t
		{
			return count;
		}

		template <typename T>
		inline typename auto DArray<T>::getSize() const noexcept -> std::size_t
		{
			return size;
		}

		template <typename T>
		inline auto DArray<T>::begin() noexcept -> iterator
		{
			return iterator{ 0, this };
		}

		template <typename T>
		inline auto DArray<T>::end() noexcept -> iterator
		{
			return iterator{ count, this };
		}

		template <typename T>
		inline auto DArray<T>::begin() const noexcept -> const_iterator
		{
			return cbegin();
		}

		template <typename T>
		inline auto DArray<T>::end() const noexcept -> const_iterator
		{
			return cend();
		}

		template <typename T>
		inline auto DArray<T>::cbegin() const noexcept -> const_iterator
		{
			return const_iterator{ 0, this };
		}

		template <typename T>
		inline auto DArray<T>::cend() const noexcept -> const_iterator
		{
			return const_iterator{ count, this };
		}

		template <typename T>
		inline bool operator==(const DArray<T>& lhs, const DArray<T>& rhs) noexcept(noexcept(std::declval<T>() == std::declval<T>()))
		{
			return std::equal(lhs.cbegin(), lhs.cend(),
							  rhs.cbegin(), rhs.cend());
		}

		template <typename T>
		inline bool operator!=(const DArray<T>& lhs, const DArray<T>& rhs) noexcept(noexcept(lhs == rhs))
		{
			return !(lhs == rhs);
		}
	}
}