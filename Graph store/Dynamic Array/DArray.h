#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <assert.h>
#include <stdexcept>
#include "../Type selector/selectBaseIterator.h"

template <typename T>
class DArray
{
private:
	static_assert(std::is_default_constructible<T>::value, "DArray<T> requires T to be default constructible.");
	static_assert(std::is_copy_assignable<T>::value, "DArray<T> requires T to be copy assignable.");

	typedef size_t unsignedInteger;

public:
	template <typename Item, bool isConst = false>
	class DArrayIterator : public selectBaseIterator<isConst, Item>::result
	{
	private:
		friend class DArray<Item>;

		typedef typename selectBaseIterator<isConst, Item>::result baseIterator;
		typedef typename typeSelector<isConst, const Item&, Item&>::result reference;
		typedef typename typeSelector<isConst, const DArray<Item>*, DArray<Item>*>::result ownerPtr;

	public:
		DArrayIterator(const DArrayIterator<Item, false>& source);
		virtual ~DArrayIterator() override = default;

		virtual reference getCurrent() const override;
		virtual void goToNext() override;
		virtual bool isFinished() const override;
		virtual baseIterator* clone() const override;

		reference operator*() const;

		DArrayIterator<Item, isConst>& operator++();
		DArrayIterator<Item, isConst> operator++(int);

		operator bool() const;
		bool operator!() const;

	private:
		DArrayIterator(unsignedInteger startPosition, ownerPtr owner);

	private:
		ownerPtr owner;
		unsignedInteger current;
	};

public:
	typedef DArrayIterator<T, false> Iterator;
	typedef DArrayIterator<T, true> ConstIterator;

	DArray();
	explicit DArray(unsignedInteger size, unsignedInteger count = 0);
	DArray(DArray<T>&& source);
	DArray(const DArray<T>& source);
	~DArray();

	DArray<T>& operator=(DArray<T>&& rhs);
	DArray<T>& operator=(const DArray<T>& rhs);

public:
	void insert(const T& item);
	void insert(const DArray<T>& other);
	void insertAt(unsignedInteger position, const T& item);
	void remove(unsignedInteger position);

	T& operator[](unsignedInteger position);
	const T& operator[](unsignedInteger position) const;

	bool isEmpty() const;
	void empty();
	void shrink(unsignedInteger size);
	void ensureSize(unsignedInteger size);

	Iterator getHeadIterator();
	ConstIterator getHeadConstIterator() const;

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

#include "DArrayImpl.hpp"
#include "DArrayIteratorImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__