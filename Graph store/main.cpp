#include <iostream>
#include "Vertex/Adjacency list/ForwardList.h"


#include "Hash\Hash.h"
#include "Hash\HashFunctionStringSpecialization.h"


#include "String\String.h"


class Item
{
public:
	Item(const char* key = "str") : key(key) {}

	bool operator==(const Item& other)const { return key == other.key; }

	const String& getKey()const { return key; }
	void setKey(const char* key) { this->key = key; }

private:
	String key;
};


class KeyExtractor
{
public:
	const String& operator()(const Item& item)const
	{
		return item.getKey();
	}
};

typedef Hash<Item, String, KeyExtractor> ItemHash;


void insertItemsToHash(ItemHash& hash, DArray<Item>& items)
{
	const int itemsCount = items.getCount();

	for (int i = 0; i < itemsCount; ++i)
		hash.insert(items[i]);
}

void setItemsWithKeys(DArray<Item>& items)
{
	static const int keysCount = 15;
	static const char* keys[] = { "some key" , "other key", "@asd" , "not very creative", "New York"
		"another key", "key 5", "123" , "123123", "running out of ideas",
		"Sofia", "IBN", "Word", "Testing", "Tired" };

	const int itemsCount = items.getCount();

	for (int i = 0; i < itemsCount && i < keysCount; ++i)
		items[i].setKey(keys[i]);
}



int main()
{
	ItemHash hash(10);

	DArray<Item> items(15, 10);
	setItemsWithKeys(items);

	insertItemsToHash(hash, items);

	const int itemsCount = items.getCount();

	for (int i = 0; i < itemsCount; ++i)
	{
		if (!(hash.getCount() == itemsCount - i))
			std::cout << "ERROR!!!!!\n";

		if (hash.remove(items[i].getKey()) == &items[i])
			std::cout << "OKAY!\n";
	}

 	return 0;
}