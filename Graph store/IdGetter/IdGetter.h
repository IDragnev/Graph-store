#ifndef __ID_GETTER_H_INCLUDED__
#define __ID_GETTER_H_INCLUDED__

namespace IDragnev::GraphStore
{
	class IdGetter
	{
	public:
		template <typename T>
		decltype(auto) operator()(const T& item) const noexcept
		{
			return idOf(item);
		}

	private:
		template <typename T>
		static auto idOf(const T& item) -> decltype(item->getID())
		{
			static_assert(noexcept(item->getID()));
			return item->getID();
		}

		template <typename T>
		static auto idOf(const T& item) -> decltype(item.getID())
		{
			static_assert(noexcept(item.getID()));
			return item.getID();
		}
	};
}

#endif //__ID_GETTER_H_INCLUDED__