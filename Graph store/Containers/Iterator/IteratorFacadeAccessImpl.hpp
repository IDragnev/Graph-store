
namespace IDragnev::Containers
{
	template <typename Iterator>
	decltype(auto) IteratorFacadeAccess::dereference(const Iterator& i)
	{
		return i.dereference();
	}

	template <typename Iterator>
	bool IteratorFacadeAccess::isValid(const Iterator& i) noexcept
	{
		return i.isValid();
	}

	template <typename Iterator>
	Iterator& IteratorFacadeAccess::increment(Iterator& i)
	{
		return i.increment();
	}

	template <typename Iterator>
	Iterator& IteratorFacadeAccess::decrement(Iterator& i)
	{
		return i.decrement();
	}

	template <typename Iterator, typename Distance>
	Iterator& IteratorFacadeAccess::advance(Iterator& i, Distance n)
	{
		return i.advance(n);
	}

	template <typename Iterator, typename Distance>
	Distance IteratorFacadeAccess::distanceBetween(const Iterator& lhs, const Iterator& rhs)
	{
		return lhs.distanceTo(rhs);
	}
}
