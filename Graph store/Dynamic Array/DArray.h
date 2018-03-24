#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <assert.h>
#include <stdexcept>

template <typename T>
class DArray
{
public:
	DArray();
	explicit DArray(int size, int count = 0);
	DArray(DArray<T>&&);
	DArray(const DArray<T>&);
	virtual ~DArray();

	DArray<T>& operator=(DArray<T>&&);
	DArray<T>& operator=(const DArray<T>&);

public:
	int getSize()const;
	int getCount()const;
	bool isEmpty()const;

	void empty();
	void ensureSize(int size);
	void shrink(int size);

public:
	virtual void insert(const T&);
	virtual void remove(int position);
	virtual int search(const T&)const;

	void insert(const DArray<T>&);
	void insertAt(int, const T&);

public:
	T& operator[](int position);
	const T& operator[](int position)const;

private:
	int count;
	int size;
	T* items;

protected:
	void setSize(int);
	void setCount(int);

	void enlargeIfFull();

	void shiftItemsOnePositionLeft(int start, int end);
	void shiftItemsOnePositionRight(int start, int end);

	T* getItems();
	const T* getItems()const;

private:
	void copyFrom(const DArray<T>&);
	void resize(int);
	void destroyAndNullMembers();
	void destroyItems();
	void nullMembers();
	void directlySetItemsCountAndSize(T* items, int count, int size);
	void throwExceptionIfInvalidIndex(int index)const;
	void moveSourceInThis(DArray& source);
};

#include "DArrayImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__