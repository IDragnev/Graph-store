#ifndef __GREATER_THAN_H_INCLUDED__
#define __GREATER_THAN_H_INCLUDED__

template <typename T>
class GreaterThan
{
public:
	bool operator()(const T& lhs, const T& rhs) const 
	{
		return lhs > rhs;
	}
};

#endif //__GREATER_THAN_H_INCLUDED__