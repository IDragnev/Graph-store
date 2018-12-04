#ifndef __ITERATOR_H_INCLUDED__
#define __ITERATOR_H_INCLUDED__

#include <type_traits>
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		template <typename T, bool isConst>
		class IteratorBase
		{
		private:
			using IteratorPtr = std::unique_ptr<IteratorBase<T, isConst>>;

		public:
			using reference = std::conditional_t<isConst, const T&, T&>;
			using pointer = std::conditional_t<isConst, const T*, T*>;

			virtual ~IteratorBase() = default;

			virtual reference operator*() const = 0;
			virtual pointer operator->() const = 0;
			virtual IteratorBase& operator++() = 0;
			virtual operator bool() const noexcept = 0;
			virtual bool operator!() const noexcept = 0;
			virtual IteratorPtr clone() const = 0;
		};

		template <typename T>
		using ConstIterator = IteratorBase<T, true>;
		template <typename T>
		using Iterator = IteratorBase<T, false>;

		template <typename Iterator, typename Function>
		void forEach(Iterator& it, Function f)
		{
			while (it)
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
		bool holdsForAll(Iterator& it, Predicate p)
		{
			using reference = typename Iterator::reference;
			return !holdsForAny(it, [&](reference item) { return !p(item); });
		}
	}
}

#endif //__ITERATOR_H_INCLUDED__
