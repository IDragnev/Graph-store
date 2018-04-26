
template <typename Integer>
specialInteger<Integer>::specialInteger() :
	value(0),
	isInfinity(true)
{
}


template <typename Integer>
specialInteger<Integer>::specialInteger(Integer value) :
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
specialInteger<Integer>& specialInteger<Integer>::operator=(Integer newValue)
{
	value = newValue;
	isInfinity = false;

	return *this;
}


template <typename Integer>
const specialInteger<Integer>& specialInteger<Integer>::getInfinity()
{
	return MY_INFINITY;
}


template <typename Integer>
specialInteger<Integer>& specialInteger<Integer>::operator+=(const specialInteger<Integer>& rhs)
{
	if (!isInfinity)
	{
		if (!rhs.isInfinity)
			value += rhs.value;
		else
			*this = MY_INFINITY;
	}

	return *this;
}


template <typename Integer>
bool specialInteger<Integer>::isEqualToInfinity() const
{
	return isInfinity;
}


template <typename Integer>
void specialInteger<Integer>::print(std::ostream& outputStream) const
{
	if (isInfinity)
		outputStream << "INFINITY";
	else
		outputStream << value;
}


template <typename Integer>
specialInteger<Integer> operator+(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs)
{
	specialInteger<Integer> temporary(lhs);

	temporary += rhs;

	return temporary;
}


template <typename Integer>
bool operator<(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs)
{
	if (!lhs.isInfinity && !rhs.isInfinity)
	{
		return lhs.value < rhs.value;
	}
	else if (!lhs.isInfinity && rhs.isInfinity)
	{
		return true;
	}
	else // INF < INF or INF < INTEGER ( < INF ) 
	{
		return false;
	}
}


template <typename Integer>
bool operator==(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs)
{
	return !(lhs.isInfinity || rhs.isInfinity) && (lhs.value == rhs.value);
}


template <typename Integer>
bool operator!=(const specialInteger<Integer>& lhs, const specialInteger<Integer>& rhs)
{
	return !(lhs == rhs);
}


template <typename Integer>
std::ostream& operator<<(std::ostream& outputStream, const specialInteger<Integer>& integer)
{
	integer.print(outputStream);

	return outputStream;
}