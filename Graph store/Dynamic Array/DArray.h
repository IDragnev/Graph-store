#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <assert.h>
#include <utility>
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

	void append(DArray<T>&&);
	void append(const DArray<T>&);

public:
	int getSize()const;
	int getCount()const;
	bool isEmpty()const;

	void empty();
	void ensureSize(int size);
	void shrink(int size);

public:
	virtual void add(T&&);
	virtual void add(const T&);
	virtual void remove(int position);
	virtual int find(const T&)const;

public:
	void addAt(int, const T&);
	void addAt(int, T&&);

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

	void resizeIfNeeded();

	void shiftOnePositionLeft(int start, int end);
	void shiftOnePositionRight(int start, int end);

	T* getItems();
	const T* getItems()const;

private:
	void copyFrom(const DArray<T>&);
	void resize(int);
	void destroyAndNullAll();
	void destroyItems();
	void nullMembers();
	void directInit(T* items, int count, int size);
};

#include "DArrayImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__