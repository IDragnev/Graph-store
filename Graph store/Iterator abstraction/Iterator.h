#ifndef __ITERATOR_H_INCLUDED__
#define __ITERATOR_H_INCLUDED__

#include <type_traits>
#include <memory>

namespace IDragnev::GraphStore
{
	namespace Detail
	{
		template <typename T, bool isConst>
		class IteratorInterface
		{
		private:
			using IteratorPtr = std::unique_ptr<IteratorInterface>;

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

#endif //__ITERATOR_H_INCLUDED__
