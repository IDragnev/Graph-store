#ifndef __ID_ACCESSOR_H_INCLUDED__
#define __ID_ACCESSOR_H_INCLUDED__

class String;

template <typename T>
class IDAccessor
{
public:
	const String& operator()(const T& item) const
	{
		return item.getKey();
	}
};

#endif // __ID_ACCESSOR_H_INCLUDED__