#ifndef __D_ARRAY_M_SEMANT_H_INCLDUED__
#define __D_ARRAY_M_SEMANT_H_INCLDUED__

#include <assert.h>
#include <stdexcept>

template <typename T>
class DArray
{
private:
	static_assert(std::is_default_constructible<T>::value, "DArray<T> requires T to be default constructible.");
	static_assert(std::is_copy_assignable<T>::value, "DArray<T> requires T to be copy assignable.");

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

	virtual void insert(const T& item);
	virtual void remove(sizeType position);

	void insert(const DArray<T>& other);
	void insertAt(sizeType position, const T& item);

	T& operator[](sizeType position);
	const T& operator[](sizeType position)const;

private:
	sizeType count;
	sizeType size;
	T* items;

protected:
	void throwExceptionIfInvalidIndex(sizeType index)const;
	void enlargeIfFull();
	
	void setCount(sizeType count);

	void shiftItemsOnePositionLeft(sizeType start, sizeType end);
	void shiftItemsOnePositionRight(sizeType start, sizeType end);
	T* getItems();

private:
	void resize(sizeType newSize);
	void copyFrom(const DArray<T>& other);
	void swapContentsWithReconstructedParameter(DArray<T> temp);
	void destroyItems();
	void nullifyMembers();
};

#include "DArrayImpl.hpp"
#endif //__D_ARRAY_M_SEMANT_H_INCLDUED__