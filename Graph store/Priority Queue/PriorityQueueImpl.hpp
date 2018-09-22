#include <assert.h>

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
HandleSetter PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::ItemAdapter::handleSetter;

template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
KeyAccessor PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::ItemAdapter::keyAccessor;


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>&
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::operator=(PriorityQueue&& rhs)
{
	if (this != &rhs)
	{
		PriorityQueue temp{ std::move(rhs) };

		using namespace std;
		swap(adapters, temp.adapters);
		swap(compareFunction, temp.compareFunction);
	}

	return *this;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(std::initializer_list<Item> source) :
	PriorityQueue{ source.begin(), source.end() }
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(InputIt first, InputIt last) :
	adapters(std::distance(first, last)) 
{
	directlyInsertAll(first, last);
	buildHeap();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
template <typename InputIt>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(InputIt first, InputIt last)
{
	auto pos = std::size_t{ 0 };
	for (; first != last; ++first)
	{
		adapters.insert(ItemAdapter{ *first, pos++ });
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::buildHeap()
{
	long lastNonLeaf = (adapters.getCount() / 2) - 1;

	while (lastNonLeaf >= 0)
	{
		siftDown(lastNonLeaf--);
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::~PriorityQueue()
{
	invalidateHandlesOfAll();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll()
{
	invalidateHandlesOfAll(std::is_same<HandleSetter, EmptyFunction<Item>>{});
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll(std::true_type)
{
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll(std::false_type)
{
	for (auto&& a : adapters)
	{
		a.invalidateHandle();
	}
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(const Item& item)
{
	auto position = adapters.getCount();
	adapters.insert(ItemAdapter{ item, position });
	siftUp(position);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
{
	auto optimal = ItemAdapter{ getOptimal() };
	moveLastToRoot();

	if (!isEmpty())
	{
		siftDown(0);
	}
	
	return std::move(optimal).wrappedItem();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline const Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
{
	assert(!isEmpty());
	return adapters[0].wrappedItem();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::moveLastToRoot()
{
	auto positionOfLast = adapters.getCount() - 1;
	moveAt(0, std::move(adapters[positionOfLast]));
	adapters.removeAt(positionOfLast);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const Handle& handle, const Key& key)
{
	static_assert(!std::is_same<HandleSetter, EmptyFunction<Item>>::value,
				  "Cannot use handle-related logic with no specific HandleSetter supplied");
	assert(handle.isValid());
	assert(hasAdapterAt(handle));

	auto& adapter = adapters[handle];
	assert(compareFunction(adapter.key(), key));
	adapter.setKey(key);
	siftUp(handle);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(std::size_t index)
{
	auto sifted = std::move(adapters[index]);

	while (hasParent(index))
	{
		auto parentIndex = getParentIndex(index);
		auto& parent = adapters[parentIndex];

		if (hasSmallerPriorityThan(parent, sifted))
		{
			moveAt(index, std::move(parent));
			index = parentIndex;
		}
		else
		{
			break;
		}
	}

	moveAt(index, std::move(sifted));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftDown(std::size_t index)
{
	auto sifted = std::move(adapters[index]);

	while (hasChildren(index))
	{
		auto optimalChildIndex = getOptimalChildIndex(index);
		auto& optimalChild = adapters[optimalChildIndex];

		if (hasSmallerPriorityThan(sifted, optimalChild))
		{
			moveAt(index, std::move(optimalChild));
			index = optimalChildIndex;
		}
		else 
		{
			break;
		}
	}

	moveAt(index, std::move(sifted));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::moveAt(std::size_t index, ItemAdapter&& adapter)
{
	assert(hasAdapterAt(index));
	adapters[index] = ItemAdapter{ std::move(adapter).wrappedItem(), index };
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasParent(std::size_t index)
{
	return index > 0;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getParentIndex(std::size_t index)
{
	return (index - 1) / 2;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasChildren(std::size_t index) const
{
	return hasAdapterAt(getLeftChildIndex(index));
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimalChildIndex(std::size_t index) const
{
	auto result = getLeftChildIndex(index);

	return hasOptimalRightSibling(result) ? result + 1 : result;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getLeftChildIndex(std::size_t index)
{
	return 2 * index + 1;
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasOptimalRightSibling(std::size_t leftChildIndex) const
{
	return hasAdapterAt(leftChildIndex + 1) && 
		   hasSmallerPriorityThan(adapters[leftChildIndex], adapters[leftChildIndex + 1]);
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const ItemAdapter& lhs, const ItemAdapter& rhs)  const
{
	return compareFunction(lhs.key(), rhs.key());
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasAdapterAt(std::size_t index) const
{
	return index < adapters.getCount();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::empty()
{
	invalidateHandlesOfAll();
	adapters.empty();
}


template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const
{
	return adapters.isEmpty();
}