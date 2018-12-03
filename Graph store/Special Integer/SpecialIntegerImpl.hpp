
namespace IDragnev
{
	template <typename Integer>
	inline SpecialInteger<Integer>::SpecialInteger() noexcept :
		value{ 0 },
		isInfinity{ true }
	{
	}

	template <typename Integer>
	inline SpecialInteger<Integer>::SpecialInteger(Integer value) noexcept :
		value{ value },
		isInfinity{ false }
	{
	}

	//
	//this overload of operator= cannot be called with an object (it will choose the defaulted overload)
	//so isInfinity must be made false
	//(isInfinity can be made true only through the defaulted overload with an obejct x such that x.isInfinity = true)
	//
	template <typename Integer>
	inline SpecialInteger<Integer>& SpecialInteger<Integer>::operator=(Integer newValue) noexcept
	{
		value = newValue;
		isInfinity = false;

		return *this;
	}

	template <typename Integer>
	inline const SpecialInteger<Integer> operator+(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		auto temp = lhs;
		temp += rhs;

		return temp;
	}

	template <typename Integer>
	SpecialInteger<Integer>& SpecialInteger<Integer>::operator+=(const SpecialInteger& rhs) noexcept
	{
		if (!isInfinity)
		{
			if (!rhs.isInfinity)
			{
				value += rhs.value;
			}
			else
			{
				*this = Infinity();
			}
		}

		return *this;
	}

	template <typename Integer>
	inline const SpecialInteger<Integer>& SpecialInteger<Integer>::Infinity() noexcept
	{
		static const SpecialInteger infinity;

		return infinity;
	}

	template <typename Integer>
	bool operator<(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		if (!lhs.isInfinity && !rhs.isInfinity)
		{
			return lhs.value < rhs.value;
		}
		else if (!lhs.isInfinity && rhs.isInfinity)
		{
			return true;
		}
		else // INF < INF or INF < INTEGER
		{
			return false;
		}
	}

	template <typename Integer>
	inline bool operator>(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		return rhs < lhs;
	}

	template <typename Integer>
	inline bool operator>=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <typename Integer>
	inline bool operator<=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		return !(lhs > rhs);
	}

	template <typename Integer>
	inline bool operator==(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		return !(lhs.isInfinity || rhs.isInfinity) && (lhs.value == rhs.value);
	}

	template <typename Integer>
	inline bool operator!=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <typename Integer>
	inline std::ostream& operator<<(std::ostream& os, const SpecialInteger<Integer>& integer)
	{
		integer.print(os);
		return os;
	}

	template <typename Integer>
	void SpecialInteger<Integer>::print(std::ostream& os) const
	{
		if (isInfinity)
		{
			os << "Infinity";
		}
		else
		{
			os << value;
		}
	}

	template <typename Integer>
	inline bool SpecialInteger<Integer>::isEqualToInfinity() const noexcept
	{
		return isInfinity;
	}
}