#ifndef __CONTAINER_ITERATOR_H_INCLUDED__
#define __CONTAINER_ITERATOR_H_INCLUDED__

#include <iterator>

namespace IDragnev::Containers
{
	template <typename Derived,
		      typename Value,
		      typename Category,
		      typename Reference = Value&,
		      typename Distance = std::ptrdiff_t
	> class IteratorFacade
	{
	public:
		using value_type = std::remove_const_t<Value>;
		using reference = Reference;
		using pointer = Value*;
		using difference_type = Distance;
		using iterator_category = Category;

		pointer operator->() const
		{
			return std::addressof(this->operator*());
		}

		reference operator*() const
		{
			return IteratorFacadeAccess::dereference(asDerived());
		}

		Derived& operator++()
		{
			return IteratorFacadeAccess::increment(asDerived());
		}

		Derived operator++(int)
		{
			auto temp = asDerived();
			++(*this);

			return temp;
		}

		Derived& operator--()
		{
			return IteratorFacadeAccess::decrement(asDerived());
		}

		Derived operator--(int)
		{
			auto temp = asDerived();
			--(*this);

			return temp;
		}

		operator bool() const noexcept
		{
			return IteratorFacadeAccess::isValid(asDerived());
		}

		bool operator!() const noexcept
		{
			return !(this->operator bool());
		}

		Derived& operator+=(Distance n)
		{
			return IteratorFacadeAccess::advance(asDerived(), n);
		}

		Derived& operator-=(Distance n)
		{
			return IteratorFacadeAccess::advance(asDerived(), n);
		}

		Derived operator+(Distance n) const
		{
			return doAdvance(asDerived(), n);
		}

		Derived operator-(Distance n) const
		{
			return doAdvance(asDerived(), n);
		}

		friend Derived operator+(Distance n, const IteratorFacade& i)
		{
			return doAdvance(i.asDerived(), n);
		}
			   
		friend Distance operator-(const IteratorFacade& lhs, const IteratorFacade& rhs)
		{
			return IteratorFacadeAccess::distanceBetween(lhs.asDerived(), rhs.asDerived());
		}

		reference operator[](Distance n) const
		{
			return IteratorFacadeAccess::dereference(*this + n);
		}
	
	private:
		static Derived doAdvance(Derived iterator, Distance n)
		{
			IteratorFacadeAccess::advance(iterator, n);
			return iterator;
		}

		Derived& asDerived() { return static_cast<Derived&>(*this); }
		const Derived& asDerived() const { return static_cast<const Derived&>(*this); };
	};

	class IteratorFacadeAccess
	{
		template <typename Derived, typename Value, typename Category,
			      typename Reference, typename Distance>
		friend class IteratorFacade;

		template <typename Iterator>
		static decltype(auto) dereference(const Iterator& i);

		template <typename Iterator>
		static bool isValid(const Iterator& i) noexcept;

		template <typename Iterator>
		static Iterator& increment(Iterator& i);

		template <typename Iterator>
		static Iterator& decrement(Iterator& i);

		template <typename Iterator, typename Distance>
		static Iterator& advance(Iterator& i, Distance d);

		template <typename Iterator, typename Distance>
		static Distance distanceBetween(const Iterator& lhs, const Iterator& rhs);
	};
}

#include "IteratorFacadeAccessImpl.hpp"
#endif //__CONTAINER_ITERATOR_H_INCLUDED__
