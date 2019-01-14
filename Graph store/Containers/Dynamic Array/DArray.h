#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <utility>
#include <assert.h>
#include <iterator>

namespace IDragnev
{
	namespace Containers
	{
		template <typename T>
		class DArray
		{
		private:
			static_assert(std::is_default_constructible_v<T>,
						  "DArray<T> requires T to be default constructible");
			static_assert(std::is_copy_assignable_v<T>,
					   	  "DArray<T> requires T to be copy assignable");

			template <typename Iter>
			using EnableIfItemIterator =
				std::enable_if_t<std::is_same_v<typename std::iterator_traits<Iter>::value_type, T>>;

			using size_type = std::size_t;

			template <typename Item, bool isConst = false>
			class DArrayIterator
			{
			private:
				friend class DArray<Item>;
				using OwnerPtr = std::conditional_t<isConst, const DArray<Item>*, DArray<Item>*>;

			public:
				using value_type = Item;
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::forward_iterator_tag;
				using reference = std::conditional_t<isConst, const Item&, Item&>;
				using pointer = std::conditional_t<isConst, const Item*, Item*>;

			public:
				DArrayIterator(const DArrayIterator<Item, false>& source) noexcept;

				pointer operator->() const;
				reference operator*() const;

				DArrayIterator<Item, isConst>& operator++();
				DArrayIterator<Item, isConst> operator++(int);

				operator bool() const noexcept;
				bool operator!() const noexcept;

				template <typename Item, bool isConst>
				friend bool operator==(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs,
									   typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs) noexcept;

			private:
				DArrayIterator(size_type startPosition, OwnerPtr owner) noexcept;

			private:
				OwnerPtr owner;
				size_type current;
			};

		public:
			using iterator = DArrayIterator<T, false>;
			using const_iterator = DArrayIterator<T, true>;

			DArray() noexcept;
			template <typename InputIt, typename = EnableIfItemIterator<InputIt>>
			DArray(InputIt first, InputIt last);
			DArray(std::initializer_list<T> source);
			explicit DArray(size_type size, size_type count = 0);
			DArray(DArray<T>&& source) noexcept;
			DArray(const DArray<T>& source);
			~DArray();

			DArray<T>& operator=(DArray<T>&& rhs) noexcept;
			DArray<T>& operator=(const DArray<T>& rhs);

		public:
			void insert(T&& item);
			void insert(const T& item);
			void insertAt(size_type position, const T& item);
			void removeAt(size_type position);

			T& operator[](size_type position);
			const T& operator[](size_type position) const;

			bool isEmpty() const noexcept;
			void empty();
			void shrink(size_type size);
			void ensureSize(size_type size);

			iterator begin() noexcept;
			iterator end() noexcept;
			const_iterator begin() const noexcept;
			const_iterator end() const noexcept;
			const_iterator cbegin() const noexcept;
			const_iterator cend() const noexcept;

			size_type getSize() const noexcept;
			size_type getCount() const noexcept;

		private:
			void enlargeIfFull();
			void resize(size_type newSize);
			void swapContentsWithReconstructedParameter(DArray<T> temp) noexcept;

			template <typename T>
			void doInsert(T&& item);

			bool hasItemAt(size_type position) const;
			void shiftItemsOnePositionLeft(size_type start, size_type end);
			void shiftItemsOnePositionRight(size_type start, size_type end);

			void setCount(size_type count);
			void destroyItems() noexcept;
			void nullifyMembers() noexcept;

		private:
			static const size_type GROWTH_FACTOR = 2;
			static const size_type DEFAULT_SIZE = 8;

		private:
			size_type count;
			size_type size;
			T* items;
		};

		template <typename Item, bool isConst>
		bool operator!=(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs,
						typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs) noexcept;

		template <typename T>
		bool operator==(const DArray<T>& lhs, const DArray<T>& rhs) noexcept(noexcept(std::declval<T>() == std::declval<T>()));
		template <typename T>
		bool operator!=(const DArray<T>& lhs, const DArray<T>& rhs) noexcept(noexcept(lhs == rhs));
	}
}

#include "DArrayImpl.hpp"
#include "DArrayIteratorImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__