#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include "..\Type selector\typeSelector.h"
#include <assert.h>
#include <stdexcept>

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
		using ownerPtr = typename TypeSelector<isConst, const DArray<Item>*, DArray<Item>*>::result;

	public:
		using value_type = Item;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		using reference = typename TypeSelector<isConst, const Item&, Item&>::result;
		using pointer = typename TypeSelector<isConst, const Item*, Item*>::result;

	public:
		DArrayIterator(const DArrayIterator<Item, false>& source);

		pointer operator->() const;
		reference operator*() const;

		DArrayIterator<Item, isConst>& operator++();
		DArrayIterator<Item, isConst> operator++(int);

		operator bool() const;
		bool operator!() const;

		template <typename Item, bool isConst>
		friend bool operator==(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs, typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs);

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

	iterator getBeginIterator();
	iterator getEndIterator();
	const_iterator getBeginConstIterator() const;
	const_iterator getEndConstIterator() const;

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
bool operator!=(typename const DArray<Item>::DArrayIterator<Item, isConst>& lhs, typename const DArray<Item>::DArrayIterator<Item, isConst>& rhs);

template <typename T>
bool operator==(const DArray<T>& lhs, const DArray<T>& rhs);
template <typename T>
bool operator!=(const DArray<T>& lhs, const DArray<T>& rhs);

template <typename T>
auto begin(DArray<T>& dArray) { return dArray.getBeginIterator(); }
template <typename T>
auto end(DArray<T>& dArray) { return dArray.getEndIterator(); }
template <typename T>
auto cbegin(const DArray<T>& dArray) { return dArray.getBeginConstIterator(); }
template <typename T>
auto cend(const DArray<T>& dArray) { return dArray.getEndConstIterator(); }

#include "DArrayImpl.hpp"
#include "DArrayIteratorImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__