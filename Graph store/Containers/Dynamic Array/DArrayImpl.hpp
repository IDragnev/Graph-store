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
			DArray<T>(source.cbegin(), source.cend())
		{
		}

		template <typename T>
		template <typename InputIt, typename>
		DArray<T>::DArray(InputIt first, InputIt last) :
			DArray(std::distance(first, last))
		{
			for (; first != last; ++first)
			{
				items[count++] = *first;
			}
		}

		template <typename T>
		DArray<T>::DArray(size_type size, size_type count) :
			count{},
			size{ size },
			items{ nullptr }
		{
			setCount(count);

			if (size > 0)
			{
				items = new T[size]{};
			}
		}

		template <typename T>
		inline void DArray<T>::setCount(size_type newCount)
		{
			assert(newCount <= size);
			count = newCount;
		}

		template <typename T>
		DArray<T>::DArray(DArray<T>&& source) noexcept :
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
		DArray<T>& DArray<T>::operator=(const DArray<T>& rhs)
		{
			if (this != &rhs)
			{
				swapContentsWithReconstructedParameter(rhs);
			}

			return *this;
		}

		template <typename T>
		DArray<T>& DArray<T>::operator=(DArray<T>&& rhs) noexcept
		{
			if (this != &rhs)
			{
				swapContentsWithReconstructedParameter(std::move(rhs));
			}

			return *this;
		}

		template <typename T>
		inline void DArray<T>::swapContentsWithReconstructedParameter(DArray<T> temporary) noexcept
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
		void DArray<T>::shrink(size_type newSize)
		{
			assert(newSize <= size);

			if (newSize == 0)
			{
				empty();
			}
			else if (newSize != size)
			{
				resize(newSize);
			}
		}

		template <typename T>
		void DArray<T>::empty()
		{
			destroyItems();
			nullifyMembers();
		}

		template <typename T>
		void DArray<T>::resize(size_type newSize)
		{
			auto newCount = (count <= newSize) ? count : newSize;
			auto temporary = DArray<T>(newSize, newCount);

			for (auto i = size_type{ 0 }; i < newCount; ++i)
			{
				temporary.items[i] = moveIfNothrowMoveAssignable(items[i]);
			}

			swapContentsWithReconstructedParameter(std::move(temporary));
		}

		template <typename T>
		inline void DArray<T>::ensureSize(size_type newSize)
		{
			if (newSize > size)
			{
				resize(newSize);
			}
		}

		template <typename T>
		void DArray<T>::insert(const DArray<T>& other)
		{
			for (auto i = size_type{ 0 }; i < other.count; ++i)
			{
				insert(other[i]);
			}
		}

		template <typename T>
		inline void DArray<T>::insert(const T& newItem)
		{
			enlargeIfFull();
			items[count++] = newItem;
		}

		template <typename T>
		inline void DArray<T>::insert(T&& newItem)
		{
			enlargeIfFull();
			items[count++] = std::move(newItem);
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
		void DArray<T>::insertAt(size_type position, const T& newItem)
		{
			if (position != count)
			{
				assert(hasItemAt(position));
				enlargeIfFull();

				shiftItemsOnePositionRight(position, count - 1);
				items[position] = newItem;
				++count;
			}
			else
			{
				insert(newItem);
			}
		}

		template <typename T>
		void DArray<T>::shiftItemsOnePositionRight(size_type start, size_type end)
		{
			assert(end + 1 < size);

			for (auto i = end + 1; i > start; --i)
			{
				items[i] = items[i - 1];
			}
		}

		template <typename T>
		inline void DArray<T>::removeAt(size_type position)
		{
			assert(hasItemAt(position));
			shiftItemsOnePositionLeft(position + 1, --count);
		}

		template <typename T>
		void DArray<T>::shiftItemsOnePositionLeft(size_type start, size_type end)
		{
			assert(start > 0 && end < size);

			for (auto i = start - 1; i < end; ++i)
			{
				items[i] = items[i + 1];
			}
		}

		template <typename T>
		inline T& DArray<T>::operator[](size_type position)
		{
			return const_cast<T&>(static_cast<const DArray<T>&>(*this)[position]);
		}

		template <typename T>
		inline const T& DArray<T>::operator[](size_type position) const
		{
			assert(hasItemAt(position));
			return items[position];
		}

		template <typename T>
		inline bool DArray<T>::hasItemAt(size_type position) const
		{
			return position < count;
		}

		template <typename T>
		inline bool DArray<T>::isEmpty() const noexcept
		{
			return count == 0;
		}

		template <typename T>
		inline typename auto DArray<T>::getCount() const noexcept -> size_type
		{
			return count;
		}

		template <typename T>
		inline typename auto DArray<T>::getSize() const noexcept -> size_type
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