#ifndef __HASH_FUNCTION__
#define __HASH_FUNCTION__

#include <functional>

/*
The Fowler–Noll–Vo hash function:
http://www.isthe.com/chongo/tech/comp/fnv/
*/

namespace IDragnev
{
	class String;
}

namespace std
{
	template <>
	struct hash<IDragnev::String>
	{
	public:
		std::size_t operator()(const IDragnev::String& key) const noexcept;

	private:
		static constexpr std::uint32_t FNV_PRIME = 16777619u;
		static constexpr std::uint32_t FNV_OFFSET_BASIS = 2166136261u;
	};
}

#endif //__HASH_FUNCTION__