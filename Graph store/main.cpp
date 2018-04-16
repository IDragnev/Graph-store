#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"


class SomeIterator : public Iterator<int>
{
	virtual ~SomeIterator() = default;

	virtual int& getCurrent() { return theMember; }
	virtual void goToNext() { ; }
	virtual bool isFinished() const { return false; }
	virtual Iterator* clone() const { return new SomeIterator(*this); }

private:
	int theMember;
};



int main()
{
	/*Iterator<int>* A = new SomeIterator;
	Iterator<int>* B = new SomeIterator;

	*A = *B;*/

 	return 0;
}