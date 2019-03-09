#ifndef __ITERATOR_H_INCLUDED__
#define __ITERATOR_H_INCLUDED__

#include <type_traits>
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		namespace Detail
		{
			template <typename T, bool isConst>
			class IteratorInterface
			{
			private:
				using IteratorPtr = std::unique_ptr<IteratorInterface<T, isConst>>;

			public:
				using reference = std::conditional_t<isConst, const T&, T&>;
				using pointer = std::conditional_t<isConst, const T*, T*>;

				virtual ~IteratorInterface() = default;

				virtual reference operator*() const = 0;
				virtual pointer operator->() const = 0;
				virtual IteratorInterface& operator++() = 0;
				virtual operator bool() const noexcept = 0;
				virtual bool operator!() const noexcept = 0;
				virtual IteratorPtr clone() const = 0;
			};
		}

		template <typename T>
		using ConstIterator = Detail::IteratorInterface<T, true>;
		template <typename T>
		using Iterator = Detail::IteratorInterface<T, false>;
	}

	namespace PolymorphicRanges
	{
		template <typename Iterator, typename Callable>
		void forEach(Iterator& it, Callable f)
		{
			while (it)
			{
				f(*it);
				++it;
			}
		}

		template <typename Iterator, typename Callable, typename Predicate>
		void forEachUntil(Iterator& it, Callable f, Predicate p)
		{
			while(it && p(*it))
			{
				f(*it);
				++it;
			}
		}

		template <typename Iterator, typename Predicate>
		bool holdsForAny(Iterator& it, Predicate p)
		{
			while (it)
			{
				if (p(*it))
				{
					return true;
				}

				++it;
			}

			return false;
		}

		template <typename Iterator, typename Predicate>
		inline bool holdsForAll(Iterator& it, Predicate p)
		{
			return !holdsForAny(it, [p](auto&& item)
			{
				return !p(std::forward<decltype(item)>(item));
			});
		}
	}
}

#endif //__ITERATOR_H_INCLUDED__
