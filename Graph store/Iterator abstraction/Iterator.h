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
			using reference = std::conditional_t<isConst, const T&, T&>;
			using pointer = std::conditional_t<isConst, const T*, T*>;
			using IteratorPtr = std::unique_ptr<IteratorBase<T, isConst>>;

		public:
			virtual ~IteratorBase() = default;

			virtual reference operator*() const = 0;
			virtual pointer operator->() const = 0;
			virtual IteratorBase& operator++() = 0;
			virtual operator bool() const = 0;
			virtual bool operator!() const = 0;
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
	}
}

#endif //__ITERATOR_H_INCLUDED__
