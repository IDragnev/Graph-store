#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <assert.h>
#include <stdexcept>

template <typename T>
class DArray
{
private:
	typedef size_t sizeType;

public:
	DArray();
	explicit DArray(sizeType size, sizeType count = 0);
	DArray(DArray<T>&& source);
	DArray(const DArray<T>& source);
	virtual ~DArray();

	DArray<T>& operator=(DArray<T>&& rhs);
	DArray<T>& operator=(const DArray<T>& rhs);

public:
	sizeType getSize()const;
	sizeType getCount()const;
	bool isEmpty()const;

	void empty();
	void ensureSize(sizeType size);
	void shrink(sizeType size);

public:
	virtual void insert(const T& item);
	virtual void remove(sizeType position);

	void insert(const DArray<T>& other);
	void insertAt(sizeType position, const T& item);

public:
	T& operator[](sizeType position);
	const T& operator[](sizeType position)const;

private:
	sizeType count;
	sizeType size;
	T* items;

protected:
	void setCount(sizeType count);
	void enlargeIfFull();

	void shiftItemsOnePositionLeft(sizeType start, sizeType end);
	void shiftItemsOnePositionRight(sizeType start, sizeType end);

	T* getItems();
	const T* getItems()const;

private:
	void resize(sizeType newSize);
	void copyFrom(const DArray<T>& other);
	void destroyAndNullMembers();
	void destroyItems();
	void nullMembers();
	void directlySetItemsCountAndSize(T* items, sizeType count, sizeType size);
	void throwExceptionIfInvalidIndex(sizeType index)const;
	void moveParameterInThis(DArray& source);
};

#include "DArrayImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__