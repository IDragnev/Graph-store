#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <assert.h>
#include <stdexcept>

template <typename T>
class DArray
{
private:
	typedef size_t size_type;

public:
	DArray();
	explicit DArray(size_type size, size_type count = 0);
	DArray(DArray<T>&&);
	DArray(const DArray<T>&);
	virtual ~DArray();

	DArray<T>& operator=(DArray<T>&&);
	DArray<T>& operator=(const DArray<T>&);

public:
	size_type getSize()const;
	size_type getCount()const;
	bool isEmpty()const;

	void empty();
	void ensureSize(size_type size);
	void shrink(size_type size);

public:
	virtual void insert(const T&);
	virtual void remove(size_type position);

	void insert(const DArray<T>&);
	void insertAt(size_type, const T&);

public:
	T& operator[](size_type position);
	const T& operator[](size_type position)const;

private:
	size_type count;
	size_type size;
	T* items;

protected:
	void setCount(size_type);
	void enlargeIfFull();

	void shiftItemsOnePositionLeft(size_type start, size_type end);
	void shiftItemsOnePositionRight(size_type start, size_type end);

	T* getItems();
	const T* getItems()const;

private:
	void setSize(size_type);
	void resize(size_type);
	void copyFrom(const DArray<T>&);
	void destroyAndNullMembers();
	void destroyItems();
	void nullMembers();
	void directlySetItemsCountAndSize(T* items, size_type count, size_type size);
	void throwExceptionIfInvalidIndex(size_type index)const;
	void moveSourceInThis(DArray& source);
};

#include "DArrayImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__