#include <iostream>
#include "Vertex/Adjacency list/ForwardList.h"

void fillListAddingTail(ForwardList<int>& list,int count, int first = 0, int step = 1)
{
	for (int i = first; i < count; i += step)
		list.addAsTail(i);
}

#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"


#include "String\String.h"


class Item
{
public:
	Item(const char* key = "asd" ) : key(key) {}

	bool operator==(const Item& other) { return key == other.key; }

	const String& getKey()const { return key; }

private:
	String key;
};


class KeyExtractor
{
public:
	const String& operator()(const Item& item)
	{
		return item.getKey();
	}
};



int main()
{
	Hash<Item, String, KeyExtractor> hash(10);
	
	Item item("asd");

	hash.insert(item);

	Item* address = hash.search("asd");
	
	if (address == &item)
		std::cout << "YES!!!" << std::endl;
	else
		std::cout << "NO!!!" << std::endl;

 	return 0;
}