#ifndef __TRAITS_H_INCLUDED__
#define __TRAITS_H_INCLUDED__

#include <type_traits>
#include <iterator>

namespace IDragnev::Traits
{
	namespace Detail
	{
		template <typename T>
		struct IsConstReferenceImpl
		{
		private:
			using RawType = std::remove_reference_t<T>;

		public:
			static constexpr bool value = std::is_const_v<RawType>;
		};

		template <typename Iterator>
		struct IsConstIteratorImpl
		{
		private:
			using reference = typename std::iterator_traits<Iterator>::reference;

		public:
			static constexpr bool value = IsConstReferenceImpl<reference>::value;
		};

		template <typename Iterator>
		struct IsForwardIteratorImpl
		{
		private:
			using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

		public:
			static constexpr bool value = std::is_convertible_v<iterator_category, std::forward_iterator_tag>;
		};
	}

	template <typename T>
	inline constexpr bool isConstReference = Detail::IsConstReferenceImpl<T>::value;

	template <typename T>
	inline constexpr bool isConstIterator = Detail::IsConstIteratorImpl<T>::value;

	template <typename T>
	inline constexpr bool isForwardIterator = Detail::IsForwardIteratorImpl<T>::value;

	template <template <typename...> typename Predicate, typename... Ts>
	inline constexpr bool allOf = (Predicate<Ts>::value && ...);
}

#endif //__TRAITS_H_INCLUDED__