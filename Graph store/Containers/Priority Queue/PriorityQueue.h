#ifndef __PRIORITY_QUEUE_H_INCLUDED__
#define __PRIORITY_QUEUE_H_INCLUDED__

#include "Priority Queue Handle\PriorityQueueHandle.h"
#include "..\..\UtilityFunctions.h"
#include "..\..\Functional\Functional.h"
#include <vector>

namespace IDragnev
{
	namespace Containers
	{
		namespace Detail
		{
			struct IdentityAccessor
			{
				template <typename T>
				constexpr const T& operator()(const T& item) const noexcept { return item; }
				template <typename T, typename U>
				constexpr void operator()(T& item, const U& key) const noexcept(noexcept(item = key)) { item = key; }
			};
		}

		template <
			typename Item,
			typename Key = Item,
			typename KeyAccessor = Detail::IdentityAccessor,
			typename CompareFunction = Functional::LessThan,
			typename HandleSetter = Functional::EmptyFunction
		> class PriorityQueue
		{
		private:
			using Handle = PriorityQueueHandle;
			
			static_assert(std::is_default_constructible_v<HandleSetter>,
						  "PriorityQueue requires HandleSetter to be default constructible");
			static_assert(std::is_default_constructible_v<KeyAccessor>,
						  "PriorityQueue requires KeyAccessor to be default constructible");
			static_assert(std::is_default_constructible_v<CompareFunction>,
						  "PriorityQueue requires CompareFunction to be default constructible");
			static_assert(std::is_copy_constructible_v<CompareFunction>,
						  "PriorityQueue requires CompareFunction to be copy constructible");
			static_assert(std::is_copy_assignable_v<CompareFunction>,
				          "PriorityQueue requires CompareFunction to be copy assignable");
			static_assert(std::is_nothrow_invocable_r_v<const Key&, KeyAccessor, const Item&>,
						  "PriorityQueue requires KeyAccessor::operator()(const Item&) to be noexcept");
			static_assert(std::is_nothrow_invocable_r_v<void, KeyAccessor, Item&, const Key&>, 
						  "PriorityQueue requires KeyAcessor::operator()(Item&, const Key&) to be noexcept");
			static_assert(std::is_nothrow_invocable_r_v<bool, CompareFunction, const Key&, const Key&>, 
						  "PriorityQueue requires ComapreFunction::operator()(const Key&, const Key&) to be noexcept");
			static_assert(std::is_nothrow_invocable_r_v<void, HandleSetter, Item&, const Handle&>,
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

			PriorityQueue& operator=(PriorityQueue&& rhs);
			PriorityQueue& operator=(const PriorityQueue& rhs);

			void insert(const Item& item);
			Item extractOptimal();
			const Item getOptimal() const;

			void improveKey(const Handle& h, const Key& key);

			bool isEmpty() const noexcept;
			void clear() noexcept;

		private:
			template <typename InputIt>
			void directlyInsertAll(InputIt first, InputIt last);
			void buildHeap();
			void siftDown(std::size_t index);
			void siftUp(std::size_t index);
			void moveAt(std::size_t index, Element&& e);
			
			Item extractRoot();
			void moveLastToRoot();
			void siftRootDown();
				
			static bool hasParent(std::size_t index) noexcept;
			static std::size_t getParentIndex(std::size_t index) noexcept;
			static std::size_t getLeftChildIndex(std::size_t index) noexcept;

			bool hasChildren(std::size_t index) const noexcept;
			std::size_t getOptimalChildIndex(std::size_t index) const noexcept;
			bool hasOptimalRightSibling(std::size_t index) const noexcept;
			bool hasSmallerPriorityThan(const Element& lhs, const Element& rhs) const noexcept;
			bool hasElementAt(std::size_t index) const noexcept;

			void invalidateHandlesOfAll() noexcept;

			void swapContentsWith(PriorityQueue temp) noexcept;

		private:
			Elements elements;
			mutable CompareFunction compareFunction;
		};
	}
}

#include "PriorityQueueImpl.hpp"
#endif //__PRIORITY_QUEUE_H_INCLUDED__