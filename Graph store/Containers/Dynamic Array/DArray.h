#ifndef __D_ARRAY_H_INCLDUED__
#define __D_ARRAY_H_INCLDUED__

#include "UtilityFunctions.h"
#include "Traits\Traits.h"
#include "Containers\Iterator\ContainerIterator.h"
#include <utility>
#include <assert.h>
#include <iterator>

namespace IDragnev::Containers
{
	template <typename T>
	class DArray
	{
	private:
		static_assert(std::is_default_constructible_v<T>,
			          "DArray<T> requires T to be default constructible");
		static_assert(std::is_copy_assignable_v<T> || std::is_nothrow_move_assignable_v<T>,
			          "DArray<T> requires T to be copy assignable or nothrow move assignable");

		template <typename Iterator>
		using EnableIfInputIterator = std::enable_if_t<Traits::isInputIterator<Iterator>>;

		template <typename Item, bool isConst = false>
		class DArrayIterator : public IteratorFacade<DArrayIterator<Item, isConst>,
                                                     std::conditional_t<isConst, const Item, Item>,
			                                         std::bidirectional_iterator_tag>
		{
		private:
			friend class DArray<Item>;
			friend class IteratorFacadeAccess;

			using OwnerPtr = std::conditional_t<isConst, const DArray<Item>*, DArray<Item>*>;
			using Ref = std::conditional_t<isConst, const Item&, Item&>;

		public:
			DArrayIterator(const DArrayIterator<Item, false>& source) noexcept;

			friend bool operator==(const DArrayIterator& lhs, const DArrayIterator& rhs) noexcept
			{
				return (lhs.owner == rhs.owner) && (lhs.current == rhs.current);
			}

			friend bool operator!=(const DArrayIterator& lhs, const DArrayIterator& rhs) noexcept
			{
				return !(lhs == rhs);
			}

		private:
			DArrayIterator(std::size_t startPosition, OwnerPtr owner) noexcept;

			Ref dereference() const;
			DArrayIterator& increment();
			DArrayIterator& decrement();
			bool isValid() const noexcept;

		private:
			OwnerPtr owner;
			std::size_t current;
		};

	public:
		using iterator = DArrayIterator<T, false>;
		using const_iterator = DArrayIterator<T, true>;

		DArray() noexcept;
		template <typename InputIt, typename = EnableIfInputIterator<InputIt>>
		DArray(InputIt first, InputIt last);
		DArray(std::initializer_list<T> source);
		explicit DArray(std::size_t size, std::size_t count = 0);
		DArray(DArray&& source) noexcept;
		DArray(const DArray& source);
		~DArray();

		DArray& operator=(DArray&& rhs) noexcept;
		DArray& operator=(const DArray& rhs);

	public:
		void insertBack(T&& item);
		void insertBack(const T& item);
		void insertAt(std::size_t position, T&& item);
		void insertAt(std::size_t position, const T& item);
		void removeAt(std::size_t position);

		T& operator[](std::size_t position);
		const T& operator[](std::size_t position) const;

		bool isEmpty() const noexcept;
		void clear() noexcept;
		void shrink(std::size_t size);
		void ensureSize(std::size_t size);

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		std::size_t getSize() const noexcept;
		std::size_t getCount() const noexcept;

	private:
		void enlargeIfFull();
		void resize(std::size_t newSize);
		void swapContentsWith(DArray temp) noexcept;

		template <typename T>
		void doInsertBack(T&& item);
		template <typename T>
		void doInsertAt(std::size_t position, T&& item);

		bool hasItemAt(std::size_t position) const noexcept;
		void shiftOnePositionLeft(std::size_t start, std::size_t end);
		void shiftOnePositionRight(std::size_t start, std::size_t end);

		void destroyItems() noexcept;
		void nullifyMembers() noexcept;

	private:
		static constexpr std::size_t GROWTH_FACTOR = 2;
		static constexpr std::size_t DEFAULT_SIZE = 8;

	private:
		std::size_t size;
		std::size_t count;
		T* items;
	};

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

#include "DArrayImpl.hpp"
#include "DArrayIteratorImpl.hpp"
#endif //__D_ARRAY_H_INCLDUED__