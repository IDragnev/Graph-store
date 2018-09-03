#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <utility>
#include <assert.h>
#include <iterator>

template <typename T>
class DArray
{
private:
	static_assert(std::is_default_constructible<T>::value, "DArray<T> requires T to be default constructible");
	static_assert(std::is_copy_assignable<T>::value, "DArray<T> requires T to be copy assignable");

	using unsignedInteger = std::size_t;

	template <typename Item, bool isConst = false>
	class DArrayIterator
	{
	private:
		friend class DArray<Item>;
		using ownerPtr = std::conditional_t<isConst, const DArray<Item>*, DArray<Item>*>;

	public:
		using value_type = Item;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		using reference = std::conditional_t<isConst, const Item&, Item&>;
		using pointer = std::conditional_t<isConst, const Item*, Item*>;

	public:
		DArrayIterator(const DArrayIterator<Item, false>& source);

		pointer operator->() const;
		reference operator*() const;

		DArrayIterator<Item, isConst>& operator++();
		DArrayIterator<Item, isConst> operator++(int);

		operator bool() const;
		bool operator!() const;

		template <typename Item, bool isConst>
		friend bool operator==(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs, 
			                   typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs);

	private:
		DArrayIterator(unsignedInteger startPosition, ownerPtr owner);

	private:
		ownerPtr owner;
		unsignedInteger current;
	};

public:
	using iterator = DArrayIterator<T, false>;
	using const_iterator = DArrayIterator<T, true>;

	DArray();
	DArray(std::initializer_list<T> source);
	explicit DArray(unsignedInteger size, unsignedInteger count = 0);
	DArray(DArray<T>&& source);
	DArray(const DArray<T>& source);
	~DArray();

	DArray<T>& operator=(DArray<T>&& rhs);
	DArray<T>& operator=(const DArray<T>& rhs);

public:
	void insert(T&& item);
	void insert(const T& item);
	void insert(const DArray<T>& other);
	void insertAt(unsignedInteger position, const T& item);
	void removeAt(unsignedInteger position);

	T& operator[](unsignedInteger position);
	const T& operator[](unsignedInteger position) const;

	bool isEmpty() const;
	void empty();
	void shrink(unsignedInteger size);
	void ensureSize(unsignedInteger size);

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	unsignedInteger getSize() const;
	unsignedInteger getCount() const;

private:
	void copyFrom(const DArray<T>& source);
	void enlargeIfFull();
	void resize(unsignedInteger newSize);
	void swapContentsWithReconstructedParameter(DArray<T> temp);
	
	bool hasItemAt(unsignedInteger position) const;
	void shiftItemsOnePositionLeft(unsignedInteger start, unsignedInteger end);
	void shiftItemsOnePositionRight(unsignedInteger start, unsignedInteger end);

	void setCount(unsignedInteger count);
	void destroyItems();
	void nullifyMembers();

private:
	static const unsignedInteger GROWTH_FACTOR = 2;
	static const unsignedInteger DEFAULT_SIZE = 8;

private:
	unsignedInteger count;
	unsignedInteger size;
	T* items;
};

template <typename Item, bool isConst>
bool operator!=(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs, 
	            typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs);

template <typename T>
bool operator==(const DArray<T>& lhs, const DArray<T>& rhs);
template <typename T>
bool operator!=(const DArray<T>& lhs, const DArray<T>& rhs);

#include "DArrayImpl.hpp"
#include "DArrayIteratorImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__