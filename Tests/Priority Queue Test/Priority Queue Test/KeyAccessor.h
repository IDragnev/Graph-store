#ifndef __KEY_ACCESSOR_H_INCLUDED__
#define __KEY_ACCESSOR_H_INCLUDED__

#include "TestItem.h"

class KeyAccessor
{
public:
	void setKey(TestItem& item, unsigned id)
	{
		item.setID(id);
	}

	unsigned getKey(const TestItem& item)
	{
		return item.getID();
	}
};

#endif //__KEY_ACCESSOR_H_INCLUDED__