/*
     MIN_TABLE_SIZE is 3 because it should be small enough to be rarely used in the constructor,
	 big enough to have empty slots for smaller counts (when the table holds 1 or 2 items),
	 and small enough to be able to halve small tables after removal (when size is 6, 7, 8)
*/

namespace IDragnev
{
	namespace Containers
	{
		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::Hash() :
			Hash(DirectSize{ MIN_TABLE_SIZE })
		{
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::Hash(DirectSize size) :
			count{ 0 },
			table{ makeEmptyTable(size.get()) }
		{
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline auto 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::makeEmptyTable(std::size_t size) -> Table
		{
			assert(size >= MIN_TABLE_SIZE);
			return Table(size, size);  //Table is zero-initialized by default
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::Hash(std::initializer_list<Item> source) :
			Hash(source.begin(), source.end())
		{
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		template <typename InputIt>
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::Hash(InputIt first, InputIt last) :
			Hash(std::distance(first, last))
		{
			for (; first != last; ++first) { insert(*first); }
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::Hash(std::size_t expectedCount) :
			Hash(DirectSize{ calculateSize(expectedCount) })
		{
		}

		//
		// ( 3 * expectedSize ) / 2 is used because if all the expected items
		// are inserted, the load factor will be 2/3 
		// 
		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline std::size_t
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::calculateSize(std::size_t expectedCount)
		{
			assert(expectedCount > 0);
			return (expectedCount < MIN_TABLE_SIZE) ? MIN_TABLE_SIZE : (3 * expectedCount) / 2;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::Hash(Hash&& source) :
			Hash{}
		{
			using std::swap;
			swap(table, source.table);
			swap(count, source.count);

			hashFunction = moveIfNothrowMoveAssignable(source.hashFunction);
			keyAccessor = moveIfNothrowMoveAssignable(source.keyAccessor);
			equalityPredicate = moveIfNothrowMoveAssignable(source.equalityPredicate);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		auto Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::operator=(Hash&& other) -> Hash&
		{
			if (this != &other)
			{
				swapContentsWith(std::move(other));
			}

			return *this;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::swapContentsWith(Hash temp)
		{
			using std::swap;
			swap(count, temp.count);
			swap(table, temp.table);
			swap(hashFunction, temp.hashFunction);
			swap(keyAccessor, temp.keyAccessor);
			swap(equalityPredicate, temp.equalityPredicate);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::insert(Item&& item)
		{
			enlargeIfFillingUp();
			fillSlot(findSlotToInsert(item), std::move(item));
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::insert(const Item& item)
		{
			enlargeIfFillingUp();
			fillSlot(findSlotToInsert(item), item);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::enlargeIfFillingUp()
		{
			if (isFillingUp())
			{
				enlarge();
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline bool Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::isFillingUp() const noexcept
		{
			return 3 * count >= 2 * table.getSize();
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::enlarge()
		{
			resize(table.getSize() * GROWTH_FACTOR);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::resize(std::size_t newSize)
		{
			//must have at least one empty position after resize
			assert(newSize >= MIN_TABLE_SIZE && newSize > count);

			auto oldState = Hash{ std::move(*this) };

			try
			{
				*this = Hash{ DirectSize{ newSize } };
				insertAllItemsFrom(oldState.table);
			}
			catch (std::bad_alloc&)
			{
				*this = Hash{ std::move(oldState) };
				throw;
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::insertAllItemsFrom(const Table& table)
		{
			for (auto&& element : table)
			{
				insertIfNotEmpty(element);
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::insertIfNotEmpty(const Element& element)
		{
			if (element)
			{
				insert(itemOf(element));
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline std::size_t
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::findSlotToInsert(const Item& item) const noexcept
		{
			auto slot = computeHashValue(keyAccessor(item));
			return findFirstEmptySlotStartingAt(slot);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline std::size_t 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::computeHashValue(const Key& key) const noexcept
		{
			return hashFunction(key) % table.getSize();
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		std::size_t 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::findFirstEmptySlotStartingAt(std::size_t slot) const noexcept
		{
			while (!isEmpty(slot))
			{
				slot = followingSlot(slot);
			}

			return slot;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline bool 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::isEmpty(std::size_t slot) const noexcept
		{
			return !static_cast<bool>(table[slot]);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline std::size_t 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::followingSlot(std::size_t slot) const noexcept
		{
			return (slot + 1) % table.getSize();
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		template <typename T>
		inline void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::fillSlot(std::size_t slot, T&& item)
		{
			table[slot] = std::forward<T>(item);
			++count;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline auto
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::search(const Key& key) const noexcept -> Element
		{
			auto slot = correspondingSlot(key);
			return (slot.has_value()) ? table[slot.value()] : Element{};
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		std::optional<std::size_t>
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::correspondingSlot(const Key& key) const noexcept
		{
			auto slot = computeHashValue(key);

			while (!isEmpty(slot))
			{
				if (match(key, table[slot]))
				{
					return { slot };
				}

				slot = followingSlot(slot);
			}

			return std::nullopt;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline bool
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::match(const Key& key, const Element& e) const noexcept
		{
			return equalityPredicate(key, keyAccessor(itemOf(e)));
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::remove(const Key& key)
		{
			auto slot = correspondingSlot(key);

			if (slot.has_value())
			{
				extractItemAt(slot.value());

				if (hasTooManyEmptySlots() && canBeShrinked())
				{
					emptySlotAndShrink(slot.value());
				}
				else
				{
					rehashClusterStartingAt(followingSlot(slot.value()));
				}
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		Item Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::extractItemAt(std::size_t slot)
		{
			assert(slot < table.getSize() && !isEmpty(slot));

			auto item = itemOf(std::move(table[slot]));
			empty(slot);
			--count;

			return item;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::empty(std::size_t slot) noexcept
		{
			table[slot] = {};
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::emptySlotAndShrink(std::size_t slot)
		{
			empty(slot);

			try
			{
				shrink();
			}
			catch (std::bad_alloc&)
			{
				rehashClusterStartingAt(followingSlot(slot));
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::shrink()
		{
			resize(table.getSize() / GROWTH_FACTOR);
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::rehashClusterStartingAt(std::size_t startingSlot)
		{
			assert(startingSlot < table.getSize());

			auto slotToEmpty = startingSlot;

			while (!isEmpty(slotToEmpty))
			{
				auto item = extractItemAt(slotToEmpty);
				insert(item);

				slotToEmpty = followingSlot(slotToEmpty);
			}
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline bool Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::hasTooManyEmptySlots() const noexcept
		{
			return 6 * count <= table.getSize();
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline bool Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::canBeShrinked() const noexcept
		{
			return table.getSize() / GROWTH_FACTOR >= MIN_TABLE_SIZE;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline void Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::empty()
		{
			*this = {};
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline bool 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::isEmpty() const noexcept
		{
			return count == 0;
		}

		template <typename Item, typename Key, typename KeyAccessor, typename HashFun, typename EqualityPredicate>
		inline std::size_t 
		Hash<Item, Key, KeyAccessor, HashFun, EqualityPredicate>::getCount() const noexcept
		{
			return count;
		}
	}
}
