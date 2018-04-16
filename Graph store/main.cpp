#include <iostream>

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"

#include "Iterator abstraction\Iterator.h"


#include "Dynamic Array\DArray.h"
#include <memory>

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

	DArray<int> darray;

	for (int i = 0; i < 10; ++i)
		darray.insert(i);

	std::unique_ptr< Iterator<int> > iterator( darray.getHeadIterator() );

	while (!iterator->isFinished())
	{
		std::cout << iterator->getCurrent() << std::endl;
		iterator->goToNext();
	}

	return 0;
}