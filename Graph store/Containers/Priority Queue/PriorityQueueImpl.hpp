#include <assert.h>

namespace IDragnev::Containers
{
	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	HandleSetter PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::Element::handleSetter;

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	KeyAccessor PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::Element::keyAccessor;

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	auto PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::operator=(PriorityQueue&& rhs) -> PriorityQueue&
	{
		if (this != &rhs)
		{
			swapContentsWith(std::move(rhs));
		}

		return *this;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	auto PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::operator=(const PriorityQueue& rhs) -> PriorityQueue&
	{
		static_assert(!std::is_pointer_v<Item>,
			          "Cannot use copy assignment when storing pointers as handles cannot be unique for each queue");

		if (this != &rhs)
		{
			swapContentsWith(rhs);
		}

		return *this;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::swapContentsWith(PriorityQueue temp) noexcept
	{
		using std::swap;
		swap(elements, temp.elements);
		swap(compareFunction, temp.compareFunction);
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(const PriorityQueue& source) :
		elements{ source.elements },
		compareFunction{ source.compareFunction }
	{
		static_assert(!std::is_pointer_v<Item>,
			          "Cannot use copy constructor when storing pointers as handles cannot be unique for each queue");
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(std::initializer_list<Item> source) :
		PriorityQueue(source.begin(), source.end())
	{
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	template <typename InputIt, typename>
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::PriorityQueue(InputIt first, InputIt last) :
		PriorityQueue()
	{
		directlyInsertAll(first, last);
		buildHeap();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	template <typename InputIt>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::directlyInsertAll(InputIt first, InputIt last)
	{
		Handle::UnderlyingType index = 0;

		for (auto current = first;
			current != last; 
			++current)
		{
			elements.emplace_back(*current, Handle{ index++ });
		}
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::buildHeap()
	{
		auto index = getLastNonLeafIndex();

		while (index >= 0)
		{
			siftDown(index--);
		}
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline auto
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getLastNonLeafIndex() const noexcept
	{
		using Utility::asSigned;
		return asSigned(elements.size() / 2u - 1u);
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftDown(std::size_t index)
	{
		auto sifted = std::move(elements[index]);

		while (hasChildren(index))
		{
			auto optimalChildIndex = getOptimalChildIndex(index);
			auto& optimalChild = elements[optimalChildIndex];

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
	inline bool
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasChildren(std::size_t index) const noexcept
	{
		return hasElementAt(getLeftChildIndex(index));
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	std::size_t PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimalChildIndex(std::size_t index) const noexcept
	{
		auto result = getLeftChildIndex(index);

		return hasOptimalRightSibling(result) ? result + 1 : result;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline std::size_t
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getLeftChildIndex(std::size_t index) noexcept
	{
		return 2 * index + 1;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline bool
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasOptimalRightSibling(std::size_t leftChildIndex) const noexcept
	{
		return hasElementAt(leftChildIndex + 1) &&
			   hasSmallerPriorityThan(elements[leftChildIndex], elements[leftChildIndex + 1]);
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline bool
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasSmallerPriorityThan(const Element& lhs, const Element& rhs)  const noexcept
	{
		return compareFunction(lhs.key(), rhs.key());
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::moveAt(std::size_t index, Element&& element)
	{
		assert(hasElementAt(index));
		elements[index] = { std::move(element).wrappedItem(), index };
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::~PriorityQueue()
	{
		invalidateHandlesOfAll();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::invalidateHandlesOfAll() noexcept
	{
		if constexpr (isHandleSetterSupplied)
		{
			for (auto& e : elements)
			{
				e.invalidateHandle();
			}
		}
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::insert(const Item& item)
	{
		auto index = elements.size();
		elements.emplace_back(item, Handle{ index });
		siftUp(index);
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftUp(std::size_t index)
	{
		auto sifted = std::move(elements[index]);

		while (hasParent(index))
		{
			auto parentIndex = getParentIndex(index);
			auto& parent = elements[parentIndex];

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
	inline bool
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasParent(std::size_t index) noexcept
	{
		return index > 0;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline std::size_t
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getParentIndex(std::size_t index) noexcept
	{
		return (index - 1) / 2;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractOptimal()
	{
		auto optimal = extractRoot();
		moveLastToRoot();
		siftRootDown();

		return optimal;
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline Item PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::extractRoot()
	{
		assert(!isEmpty());
		return std::move(elements[0]).wrappedItem();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::moveLastToRoot()
	{
		moveAt(0, std::move(elements.back()));
		elements.pop_back();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::siftRootDown()
	{
		if (!isEmpty())
		{
			siftDown(0);
		}
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline const Item
	PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::getOptimal() const
	{
		assert(!isEmpty());
		return elements[0].wrappedItem();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::isEmpty() const noexcept
	{
		return elements.empty();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::improveKey(const Handle& handle, const Key& key)
	{
		static_assert(isHandleSetterSupplied,
			          "Cannot use handle-related logic with no specific HandleSetter supplied");
		assert(handle.isValid);
		assert(hasElementAt(handle));

		auto& element = elements[handle];
		assert(compareFunction(element.key(), key));
		element.setKey(key);
		siftUp(handle);
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	inline bool PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::hasElementAt(std::size_t index) const noexcept
	{
		return index < elements.size();
	}

	template <typename Item, typename Key, typename KeyAccessor, typename CompareFunction, typename HandleSetter>
	void PriorityQueue<Item, Key, KeyAccessor, CompareFunction, HandleSetter>::clear() noexcept
	{
		invalidateHandlesOfAll();
		elements.clear();
	}
}