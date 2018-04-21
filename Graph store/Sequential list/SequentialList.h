#ifndef __SEQUENTIAL_LIST_H_INCLUDED__
#define __SEQUENTIAL_LIST_H_INCLUDED__

#include "../Iterator abstraction/Iterator.h"

template <typename T>
class SequentialList
{
public:
	virtual ~SequentialList() = default;

	virtual size_t getCount() const = 0;
	virtual bool isEmpty() const = 0;
	virtual void empty() = 0;

	virtual Iterator<T>* getIterator() = 0;

	virtual void insertFirst(const T& item) = 0;
	virtual void insertLast(const T& item) = 0;

	virtual void removeFirst() = 0;
	virtual void removeLast() = 0;
};

#endif //__SEQUENTIAL_LIST_H_INCLUDED__
