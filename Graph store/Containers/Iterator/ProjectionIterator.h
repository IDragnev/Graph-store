#ifndef __PROJECTION_ITERATOR_H_INCLUDED__
#define __PROJECTION_ITERATOR_H_INCLUDED__

#include "ContainerIterator.h"

namespace IDragnev::Containers
{
	template <typename Iterator, typename Extractor>
	class ProjectionIterator : public IteratorFacade<ProjectionIterator<Iterator, Extractor>,
                                                     std::decay_t<std::invoke_result_t<Extractor, const Iterator&>>,
												     typename std::iterator_traits<Iterator>::iterator_category,
													 std::invoke_result_t<Extractor, const Iterator&>,
													 typename std::iterator_traits<Iterator>::difference_type>
	{
	private:
		using Reference = std::invoke_result_t<Extractor, const Iterator&>;
		using Distance = typename std::iterator_traits<Iterator>::difference_type;
		
		static_assert(std::is_lvalue_reference_v<Reference>,
			          "ProjectionIterator requires Extractor to return an lvalue reference");
		static_assert(std::is_copy_constructible_v<Extractor> && std::is_copy_assignable_v<Extractor>,
			          "ProjectionIterator requires Extractor to be copy constructible and copy assignable");

		friend class IteratorFacadeAccess;

	public:
		explicit ProjectionIterator(Iterator it, Extractor extractor = {}) :
			iterator(it),
			extractor(extractor)
		{
		}

	private:
		bool isValid() const noexcept
		{
			return static_cast<bool>(iterator);
		}

		Reference dereference() const
		{
			return extractor(iterator);
		}

		ProjectionIterator& increment()
		{
			++iterator;
			return *this;
		}

		ProjectionIterator& decrement()
		{
			--iterator;
			return *this;
		}

		ProjectionIterator& advance(Distance n)
		{
			iterator += n;
			return *this;
		}

		Distance distanceTo(const ProjectionIterator& other) const
		{
			return iterator - other.iterator;
		}

	private:
		Iterator iterator;
		Extractor extractor;
	};

	template <typename Iterator, typename Extractor>
	inline auto project(Iterator it, Extractor extractor)
	{
		return ProjectionIterator<Iterator, Extractor>(it, extractor);
	}
}

#endif //__PROJECTION_ITERATOR_H_INCLUDED__
