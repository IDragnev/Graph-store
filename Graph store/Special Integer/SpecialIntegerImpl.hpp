
template <typename Integer>
SpecialInteger<Integer>::SpecialInteger() :
	value(0),
	isInfinity(true)
{
}


template <typename Integer>
SpecialInteger<Integer>::SpecialInteger(Integer value) :
	value(value),
	isInfinity(false)
{
}


//
//this overload of operator= cannot be called with an object (it will choose the defaulted overload)
//so isInfinity must be made false
//(isInfinity can be made true only through the defaulted overload with an obejct x such that x.isInfinity = true)
//
template <typename Integer>
SpecialInteger<Integer>& SpecialInteger<Integer>::operator=(Integer newValue)
{
	value = newValue;
	isInfinity = false;

	return *this;
}


template <typename Integer>
const SpecialInteger<Integer>& SpecialInteger<Integer>::Infinity()
{
	static const SpecialInteger<Integer> infinity;

	return infinity;
}


template <typename Integer>
SpecialInteger<Integer>& SpecialInteger<Integer>::operator+=(const SpecialInteger<Integer>& rhs)
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
bool SpecialInteger<Integer>::isEqualToInfinity() const
{
	return isInfinity;
}


template <typename Integer>
void SpecialInteger<Integer>::print(std::ostream& outputStream) const
{
	if (isInfinity)
	{
		outputStream << "INFINITY";
	}
	else
	{
		outputStream << value;
	}
}


template <typename Integer>
SpecialInteger<Integer> operator+(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs)
{
	SpecialInteger<Integer> temporary(lhs);
	temporary += rhs;

	return temporary;
}


template <typename Integer>
bool operator<(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs)
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
bool operator>(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs)
{
	return rhs < lhs;
}


template <typename Integer>
bool operator==(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs)
{
	return !(lhs.isInfinity || rhs.isInfinity) && (lhs.value == rhs.value);
}


template <typename Integer>
bool operator!=(const SpecialInteger<Integer>& lhs, const SpecialInteger<Integer>& rhs)
{
	return !(lhs == rhs);
}


template <typename Integer>
std::ostream& operator<<(std::ostream& outputStream, const SpecialInteger<Integer>& integer)
{
	integer.print(outputStream);

	return outputStream;
}