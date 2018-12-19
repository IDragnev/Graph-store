#ifndef __PRIORITY_QUEUE_H_INCLUDED__
#define __PRIORITY_QUEUE_H_INCLUDED__

#include "Priority Queue Handle\PriorityQueueHandle.h"
#include "..\..\UtilityFunctions.h"
#include "..\..\Traits\Traits.h"
#include <vector>

namespace IDragnev
{
	namespace Containers
	{
		struct EmptyFunction
		{
			template <typename... Args>
			constexpr void operator()(Args&&...) const noexcept { }
		};

		struct IdentityAccessor
		{
			template <typename T>
			const T& operator()(const T& item) const noexcept { return item; }
			template <typename T, typename U>
			void operator()(T& item, const U& key) const noexcept(noexcept(item = key)) { item = key; }
		};

		template <
			typename Item,
			typename Key = Item,
			typename KeyAccessor = IdentityAccessor,
			typename CompareFunction = LessThan,
			typename HandleSetter = EmptyFunction
		> class PriorityQueue
		{
		private:
			using Handle = PriorityQueueHandle;
			
			static_assert(Traits::IsNothrowInvocable<KeyAccessor, const Item&>::value,
						  "PriorityQueue requires KeyAccessor::operator()(const Item&) to be noexcept");
			static_assert(Traits::IsNothrowInvocable<KeyAccessor, Item&, const Key&>::value, 
						  "PriorityQueue requires KeyAcessor::operator()(Item&, const Key&) to be noexcept");
			static_assert(Traits::IsNothrowInvocable<CompareFunction, const Key&, const Key&>::value, 
						  "PriorityQueue requires ComapreFunction::operator()(const Key&, const Key&) to be noexcept");
			static_assert(Traits::IsNothrowInvocable<HandleSetter, Item&, const Handle&>::value,
						  "PriorityQueue requires HandleSetter::operator()(Item&, const PriorityQueueHandle&) to be noexcept");

			class Element
			{
			public:
				Element(Item&& item, const Handle& h = {}) : item{ std::move(item) } { setHandle(h); }
				Element(const Item& item, const Handle& h = {}) : item{ item } { setHandle(h); }

				const Key& key() const { return keyAccessor(item); }
				void setKey(const Key& key) { keyAccessor(item, key); }

				const Item& wrappedItem() const & { return item; }
				Item wrappedItem() && { return std::move(item); }

				void invalidateHandle() { setHandle({}); }

			private:
				void setHandle(const Handle& h) { handleSetter(item, h); }

			private:
				static HandleSetter handleSetter;
				static KeyAccessor keyAccessor;

			private:
				Item item;
			};

			using Elements = std::vector<Element>;

		public:
			PriorityQueue() = default;
			template <typename InputIt>
			PriorityQueue(InputIt first, InputIt last);
			PriorityQueue(std::initializer_list<Item> source);
			PriorityQueue(PriorityQueue&& source) = default;
			PriorityQueue(const PriorityQueue& source);
			~PriorityQueue();

			PriorityQueue& operator=(PriorityQueue&& rhs) noexcept(std::is_nothrow_move_assignable_v<Elements>);
			PriorityQueue& operator=(const PriorityQueue& rhs);

			void insert(const Item& item);
			Item extractOptimal();
			const Item getOptimal() const noexcept(std::is_nothrow_copy_constructible_v<Item>);

			void improveKey(const Handle& h, const Key& key);

			bool isEmpty() const noexcept;
			void empty() noexcept;

		private:
			template <typename InputIt>
			void directlyInsertAll(InputIt first, InputIt last);
			void buildHeap();
			void siftDown(std::size_t index);
			void siftUp(std::size_t index);
			void moveAt(std::size_t index, Element&& e);
			void moveLastToRoot();

			static bool hasParent(std::size_t index) noexcept;
			static std::size_t getParentIndex(std::size_t index) noexcept;
			static std::size_t getLeftChildIndex(std::size_t index) noexcept;

			bool hasChildren(std::size_t index) const noexcept;
			std::size_t getOptimalChildIndex(std::size_t index) const noexcept;
			bool hasOptimalRightSibling(std::size_t index) const noexcept;
			bool hasSmallerPriorityThan(const Element& lhs, const Element& rhs) const noexcept;
			bool hasElementAt(std::size_t index) const noexcept;

			void invalidateHandlesOfAll() noexcept;
			void invalidateHandlesOfAll(std::true_type) noexcept;
			void invalidateHandlesOfAll(std::false_type) noexcept;

			void swapContentsWithReconstructedParameter(PriorityQueue temporary) noexcept;

		private:
			Elements elements;
			mutable CompareFunction compareFunction;
		};
	}
}

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__